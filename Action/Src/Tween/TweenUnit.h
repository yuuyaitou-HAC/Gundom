#ifndef TWEEN_UNIT_H_
#define TWEEN_UNIT_H_

#include <string>
#include <functional>
#include <gslib.h>
#include <GSeasing.h>

//イージング種別
enum class EaseType {
    Linear,
    EaseInBack,
    EaseInCirc,
    EaseInCubic,
    EaseInExpo,
    EaseInQuad,
    EaseInQuart,
    EaseInQuint,
    EaseInSine,
    EaseOutBack,
    EaseOutBounce,
    EaseOutCirc,
    EaseOutCubic,
    EaseOutElastic,
    EaseOutExpo,
    EaseOutQuad,
    EaseOutQuart,
    EaseOutQuint,
    EaseOutSine,
    EaseInOutBack,
    EaseInOutCirc,
    EaseInOutCubic,
    EaseInOutExpo,
    EaseInOutQuad,
    EaseInOutQuart,
    EaseInOutQuint,
    EaseInOutSine,
    Punch,

};

//Tweenの動作の一つを管理するクラスの基底クラス
class TweenUnit {
public:

	//コンストラクタ
	TweenUnit(float duration);

	//仮想デストラクタ
	virtual ~TweenUnit() = default;

	//更新
	void update(float delta_time);

	//終了したか
	bool is_Finishd() const;

    //イージング種別を設定する
    TweenUnit& ease(EaseType ease_type);


    TweenUnit& overshoot(float overshoot);

    //開始遅延時間を設定する
    TweenUnit& delay(float delay);

    //完了後のコールバック
    TweenUnit& on_complete(std::function<void()> complete_func);

    //名前を取得する
    const std::string& name() const;

    //名前を設定する
    TweenUnit& name(const std::string name);

	//コピー禁止
	TweenUnit(const TweenUnit& other) = delete;
	TweenUnit& operator = (const TweenUnit& other) = delete;

protected:
	//派生クラスごとの更新処理
	virtual void on_update(float progress) {}

private:

	//アニメーション時間
	float duration_{ 0.0f };

	//経過時間
	float elapsed_{ 0.0f };

	//終了したか
	bool is_finished_{ false };

    //イージング種別
    EaseType ease_type_{ EaseType::Linear };

    //勢い
    float overshoot_{ 1.70158f };

    //開始遅延時間
    float delay_{ 0.0f };

    //完了時のコールバック
    std::function<void()>complete_func_{};

    //名前
    std::string name_{};

    //値にイージングを適応する
    static float apply_ease(float value, EaseType ease_type, float overshoot);
};


#endif // !TWEEN_UNIT_H_
