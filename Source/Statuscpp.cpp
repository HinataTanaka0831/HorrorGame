#include "Status.h"

Status::Status(int hp, int attack)
	: mnHp(hp)
	, mnMaxHp(hp)
	, mnAttack(attack)
{
}

Status::~Status()
{
}

// 被ダメージによるHP減少（0未満へのアンダーフロー防止ガード付き）
// 入力: damage(ダメージ量) / 出力: なし / 副作用: mnHpの減算
void Status::Damage(int damage)
{
	mnHp -= damage;
	if (mnHp <= 0)
	{
		mnHp = 0;
	}
}