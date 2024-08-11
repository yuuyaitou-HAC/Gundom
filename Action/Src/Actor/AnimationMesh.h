#ifndef ANIMATION_MESH_H_
#define ANIMATION_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>


//アニメーションイベント構造
struct AnimationEvent
{
	// コンストラクタ
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()> callback, bool isExecuted) :
		m_motion{ motion }, m_time{ time }, m_callback{ callback }, m_isExecuted{false} {}
	// イベントを発生させるモーション番号
	GSuint m_motion;
	// イベントを発生させるタイミング
	GSfloat m_time;
	// イベント発生時のコールバック
	std::function<void()> m_callback;
	// 実行したか
	bool m_isExecuted;
};


// アニメーション付きメッシュクラス
class AnimationMesh {
public:
	// コンストラクタ
	AnimationMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true, GSuint layerCount = 1, GSuint boneCount = 256);
	// 更新
	void Update(float delta_time);
	// 描画
	void Draw() const;

	// モーションの変更（レイヤー指定なし）
	void ChangeMotion(GSuint motion, bool loop = true, bool restart = false);
	// レイヤーインデックスの設定
	void SetLayerIndices(GSuint layerIndex, const GSuint layerBones[], GSuint layerBonesCount);

	// 変換行列を設定する
	void Transform(const GSmatrix4& matrix);
	// モーションの終了時間を取得
	float MotionEndTime(GSuint layer = 0) const;
	// モーションが終了しているか？
	bool IsMotionEnd(GSuint layer = 0) const;
	// ボーンのワールド変換行列を取得
	GSmatrix4 BoneMatrices(int boneNo) const;
	// 現在のモーションの再生時間を取得
	float CurrentMotionTime(GSuint layer = 0) const;
	// 現在のモーションの再生時間を設定
	void CurrentMotionTime(float time, GSuint layer = 0);
	// アニメーションイベントを登録（レイヤー指定なし）
	void AddEvent(GSuint motion, GSfloat time, std::function<void()> callback);
	// アニメーションイベントを登録（レイヤー指定あり）
	void AddEvent(GSuint layer, GSuint motion, GSfloat time, std::function<void()> callback);

private:
	// アニメーションクラス
	class Animation {
	public:
		// コンストラクタ
		Animation(GSuint animation = 0, GSuint motion = 0, bool loop = true);
		// デストラクタ
		~Animation();
		// 更新
		void Update(float deltaTime);
		// モーションの変更
		void ChangeMotion(GSuint motion, bool loop = true, bool restart = false);
		// モーションの最終時間を取得
		float MotionEndTime() const;
		// 現在再生中のモーションタイマを取得
		float CurrentMotionTime() const;
		// 現在再生中のモーションタイマを設定
		void CurrentMotionTime(float time);
		// モーションが終了しているか？
		bool IsMotionEnd() const;
		// ボーンのローカル変換行列を取得
		GSmatrix4 LocalBoneMatrix(GSuint boneNo) const;
		// ボーン数を返す
		GSuint BoneCount() const;
		// イベントを追加
		void AddEvent(GSuint motion, GSfloat time, std::function<void()> callback);
		// ループ処理
		void Loop(float deltaTime);
		// イベント処理
		void Event();

	private:
		// アニメーション
		GSuint      m_animation{ 0 };
		// モーション番号
		GSuint      m_motion{ 0 };
		// モーションタイマ
		GSfloat     m_motionTimer{ 0.0f };
		// モーションループフラグ
		bool        m_motionLoop{ true };
		// 前回再生したモーション番号
		GSuint      m_prevMotion{ 0 };
		// 前回再生した最終アニメーションタイマ
		GSfloat     m_prevMotionTimer{ 0.0f };
		// 補間アニメーションタイマ
		GSfloat     m_lerpTimer{ 0.0f };
		// アニメーションイベントのコンテナ
		std::vector<AnimationEvent*> m_events;
	};
	// メッシュ
	GSuint                  m_mesh;
	// スケルトン
	GSuint                  m_skeleton;
	// アニメーションレイヤー
	std::vector<Animation>  m_animationLayers;
	// アニメーションレイヤーインデックス
	std::vector<GSuint>     m_animationLayerIndices;
	// ボーンのローカル変換行列
	std::vector<GSmatrix4>  m_localBoneMatrices;
	// ボーンの変換行列
	std::vector<GSmatrix4>  m_boneMatrices;
	// 座標変換行列
	GSmatrix4               m_transform;
	// α値
	float                   m_alpha;
};
#endif // !ANIMATION_MESH_H_


