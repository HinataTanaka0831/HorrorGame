#include "Collision.h"

bool Collision::CheckCircleToCircle(
	const VECTOR& centerPositionA,
	const float& radiusA,
	const VECTOR& centerPositionB,
	const float& radiusB)
{
	// “–‚½‚è”»’è‚ğ‘‚­@O•½•û‚Ì’è—(a^2 + b^2 = c^2)

	// X²‚Ì‹——£i—Î‚Ì‰¡üj
	float distanceX = centerPositionA.x - centerPositionB.x;
	// Y²‚Ì‹——£i—Î‚Ìcüj
	float distanceY = centerPositionA.y - centerPositionB.y;
	// Z²‚Ì‹——£i—Î‚Ì‰œ‚Ìüj
	float distanceZ = centerPositionA.z - centerPositionB.z;

	// ğŒ
	float powX = distanceX * distanceX; // ‰¡ü‚Ì2æ
	float powY = distanceY * distanceY; // cü‚Ì‚Qæ
	float powZ = distanceZ * distanceZ; // ‰œ‚Ìü‚Ì2æ
	float powR = (radiusA + radiusB) * (radiusA + radiusB);  // ‚»‚ê‚¼‚ê‚Ì”¼Œa‚ğ‘«‚µ‚½‚à‚Ì‚Ì‚Qæ
	if ((powX + powY + powZ) <= powR)
	{
		return true;       // ¬Œ÷
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}

bool Collision::CheckPointToCircle(
	const VECTOR& pointPosition,
	const VECTOR& centerPosition,
	const float& radius
)
{
	// X²‚Ì‹——£
	float distanceX = centerPosition.x - pointPosition.x;
	// Y²‚Ì‹——£
	float distanceY = centerPosition.y - pointPosition.y;
	// Z²‚Ì‹——£
	float distanceZ = centerPosition.z - pointPosition.z;

	// ”»’è
	float powX = distanceX * distanceX;   // ‰¡ü‚Ì‚Qæ
	float powY = distanceY * distanceY;   // cü‚Ì‚Qæ
	float powZ = distanceZ * distanceZ;   // ‰œ‚Ìü‚Ì2æ
	float powR = radius * radius;         // ”¼Œa‚Ì‚Qæ

	if ((powX + powY + powZ) <= powR)
	{
		return true;    // “–‚½‚Á‚Ä‚¢‚é
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}
