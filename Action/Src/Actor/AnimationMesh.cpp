#include "Actor/AnimationMesh.h"
#include <algorithm>
#include <cmath>

//補間フレーム数
const GSfloat LerpTime{ 10.0f };


//コンストラクタ
AnimationMesh::AnimationMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion, bool loop, GSuint layer_count, GSuint bone_count) :
	m_mesh{ mesh },
	m_skeleton{ skeleton },
	m_animationLayers(layer_count, Animation{ animation,motion,loop }),
	m_animationLayerIndices(bone_count, 0),
	m_localBoneMatrices(bone_count, GS_MATRIX4_IDENTITY),
	m_boneMatrices(bone_count, GS_MATRIX4_IDENTITY),
	m_transform{ GS_MATRIX4_IDENTITY },
	m_alpha{ 1.0f }
{}


//更新
void AnimationMesh::Update(float deltaTime)
{
	//アニメーションレイヤーを更新
	for (auto& layer : m_animationLayers)
	{
		layer.Update(deltaTime);
	}
}

//描画
void AnimationMesh::Draw() const
{
	// スケルトンの自動計算を無効にする
	gsDisable(GS_CALC_SKELETON);
	// スケルトンのバインド
	gsBindSkeleton(m_skeleton);
	// ボーンのワールド変換行列を設定
	gsSetMatrixSkeleton(m_boneMatrices.data());

	glPushMatrix();
	// 座標変換行列を設定する
	glMultMatrixf(m_transform);
	// メッシュの描画
	gsDrawMesh(m_mesh);
	glPopMatrix();

	// スケルトンの自動計算を有効にする
	gsEnable(GS_CALC_SKELETON);
}

//モーション変更
void AnimationMesh::ChangeMotion(GSuint motion, bool loop, bool restart)
{
	for (auto& layer : m_animationLayers)
	{
		layer.ChangeMotion(motion, loop, restart);
	}
}

//レイヤーインデックスの設定
void AnimationMesh::SetLayerIndices(GSuint layerIndex, const GSuint layerBones[], GSuint layerBonesCount)
{
	for (GSuint i = 0; i < layerBonesCount; ++i)
	{
		m_animationLayerIndices[layerBones[i]] = layerIndex;
	}
}

//変換行列の設定
void AnimationMesh::Transform(const GSmatrix4& matrix)
{
	// 座標変換行列を更新
	m_transform = matrix;
	// 各レイヤーのからローカル変換行列を取得
	for (GSuint i = 0; i < gsGetSkeletonNumBones(m_skeleton); ++i)
	{
		m_localBoneMatrices[i] = m_animationLayers[m_animationLayerIndices[i]].LocalBoneMatrix(i);
	}
	// ボーンの変換行列を計算
	gsBindSkeleton(m_skeleton);
	gsCalculateSkeleton(NULL, m_localBoneMatrices.data(), m_boneMatrices.data());
}

//モーションの終了時間
float AnimationMesh::MotionEndTime(GSuint layer) const
{
	return m_animationLayers[layer].MotionEndTime();
}

//モーションが終了しているか
bool AnimationMesh::IsMotionEnd(GSuint layer) const
{
	return m_animationLayers[layer].IsMotionEnd();
}

//ボーンのワールド変換行列を取得
GSmatrix4 AnimationMesh::BoneMatrices(int boneNo) const
{
	return m_boneMatrices[boneNo] * m_transform;
}

//モーションの再生時間を取得
float AnimationMesh::CurrentMotionTime(GSuint layer) const
{
	return m_animationLayers[layer].CurrentMotionTime();
}

//現在のモーションの再生時間を設定
void AnimationMesh::CurrentMotionTime(float time, GSuint layer)
{
	m_animationLayers[layer].CurrentMotionTime(time);
}

//アニメーションイベントの追加(レイヤー指定なし)
void AnimationMesh::AddEvent(GSuint motion, GSfloat time, std::function<void()> callback)
{
	for (auto& layer : m_animationLayers)
	{
		layer.AddEvent(motion, time, callback);
	}
}

//アニメーションイベントの追加(レイヤー指定あり)
void AnimationMesh::AddEvent(GSuint layer, GSuint motion, GSfloat time, std::function<void()> callback)
{
	m_animationLayers[layer].AddEvent(motion, time, callback);
}



//アニメーション

//コンストラクタ
AnimationMesh::Animation::Animation(GSuint animation, GSuint motion, bool loop) :
	m_animation{ animation },
	m_motion{ motion },
	m_motionTimer{ 0.0f },
	m_motionLoop{ loop },
	m_prevMotion{ motion },
	m_prevMotionTimer{ 0.0f },
	m_lerpTimer{ 0.0f }
{}


//デストラクタ
AnimationMesh::Animation::~Animation()
{
	for (auto event : m_events)
	{
		delete event;
		event = nullptr;
	}
}

//更新
void AnimationMesh::Animation::Update(float deltaTime)
{
	//アニメーションタイマの更新
	m_motionTimer += deltaTime;
	Event();
	if (m_motionLoop) Loop(deltaTime);
	else m_motionTimer = std::min(m_motionTimer, MotionEndTime() - 1.0f); //モーションタイマのクランプ

	//補間タイマの更新
	m_lerpTimer = std::min(m_lerpTimer + deltaTime, LerpTime);
}


//モーションの変更
void AnimationMesh::Animation::ChangeMotion(GSuint motion, bool loop, bool restart)
{
	// 現在と同じモーションの場合は何もしない
	if (m_motion == motion && !restart) return;
	// 補間中の前半は、前回のモーションを更新をしないようにする
	if (m_lerpTimer > (LerpTime * 0.5f)) {
		// 前回のモーション番号とモーション時間を保存
		m_prevMotion = m_motion;
		m_prevMotionTimer = m_motionTimer;
		// 補間中タイマの初期化 
		m_lerpTimer = 0.0f;
	}
	// モーションの更新
	m_motion = motion;
	// モーションタイマの初期化
	m_motionTimer = 0.0f;
	// ループフラグの設定
	m_motionLoop = loop;
	std::for_each(m_events.begin(), m_events.end(),
		[](AnimationEvent* event) {event->m_isExecuted = false; });
}

//モーションの終了時間を取得
float AnimationMesh::Animation::MotionEndTime() const
{
	return gsGetEndAnimationTime(m_animation, m_motion);
}

//モーションの再生時間を取得
float AnimationMesh::Animation::CurrentMotionTime() const
{
	return m_motionTimer;
}

//再生中のモーションタイマを設定
void AnimationMesh::Animation::CurrentMotionTime(float time)
{
	m_motionTimer = time;
}

//モーションが終了しているか
bool AnimationMesh::Animation::IsMotionEnd() const
{
	// ループモーションは終了しない
	if (m_motionLoop) return false;
	// 終了しているか？
	return m_motionTimer >= (MotionEndTime() - 1.0f);
}

//ローカル変換行列を取得
GSmatrix4 AnimationMesh::Animation::LocalBoneMatrix(GSuint boneNo) const
{
	GSmatrix4 result;
	// 補間付きアニメーション変換行列を計算
	gsCalculateBoneAnimationLerp(
		m_animation, m_prevMotion, m_prevMotionTimer,
		m_animation, m_motion, m_motionTimer,
		boneNo,
		m_lerpTimer / LerpTime,
		&result
	);
	return result;
}

//ボーンの数を返す
GSuint AnimationMesh::Animation::BoneCount() const
{
	return gsGetAnimationNumBones(m_animation, m_motion);
}

//アニメーションイベントの登録	
void AnimationMesh::Animation::AddEvent(GSuint motion, GSfloat time, std::function<void()> callback)
{
	m_events.push_back(new AnimationEvent(motion, time, callback, false));
}

//ループ処理
void AnimationMesh::Animation::Loop(float deltaTime)
{
	if (m_motionTimer >= MotionEndTime())
	{
		// モーションタイマをループさせる
		m_motionTimer -= MotionEndTime();
		std::for_each(m_events.begin(), m_events.end(),
			[](AnimationEvent* event) {event->m_isExecuted = false; });
	}
}

//イベント
void AnimationMesh::Animation::Event()
{
	GSuint count = BoneCount();
	// 全イベントをチェックし、必要であればイベントを発行する
	for (auto& event : m_events)
	{
		// 実行済みか？
		if (event->m_isExecuted) continue;
		// 対象のモーション番号か？
		if (event->m_motion != m_motion) continue;
		// 今フレームで到達しているか？
		if (event->m_time <= m_motionTimer)
		{
			event->m_callback();
			event->m_isExecuted = true;
		}
	}
}