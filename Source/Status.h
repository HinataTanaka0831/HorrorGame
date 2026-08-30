#pragma once

// キャラクターの体力（HP）および攻撃力パラメータを保持・更新する構造体
class Status
{
public:
	// HPおよび最大HP、攻撃力の初期化
	// 入力: hp(初期HP), attack(攻撃力) / 出力: なし / 副作用: メンバ変数初期化
	Status(int hp, int attack);
	~Status();

	// 被ダメージによるHP減少（0未満へのアンダーフロー防止ガード付き）
	// 入力: damage(ダメージ量) / 出力: なし / 副作用: mnHpの減算
	void Damage(int damage);

	int GetHp() const { return mnHp; }
	int GetMaxHp() const { return mnMaxHp; }
	int GetAttack() const { return mnAttack; }

private:
	int mnHp;
	int mnMaxHp;
	int mnAttack;
};