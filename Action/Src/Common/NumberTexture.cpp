#include"NumberTexture.h"
#include <sstream>
#include <iomanip>

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
NumberTexture::NumberTexture(GSuint texture, int width, int height) :
	texture_{ texture }, width_{ width }, height_{ height }{
}

// •`‰æ (‰E‹l‚ß)
void NumberTexture::draw(const GSvector2& position, int num, int digit, char fill)const {
	std::stringstream ss;
	ss << std::setw(digit) << std::setfill(fill) << num;
	draw(position, ss.str());
}

// •`‰æ (¶‹l‚ß)
void NumberTexture::draw(const GSvector2& position, int num)const {
	draw(position, std::to_string(num));
}

// •`‰æ
void NumberTexture::draw(const GSvector2 position, const std::string& num)const {
	// ”š‚ğ1•¶š‚¸‚Â•`‰æ‚·‚é
	for (int i = 0; i < (int)num.size(); ++i) {
		if (num[i] == ' ')continue;// ‹ó”’•¶š‚ÍƒXƒLƒbƒv
		// charŒ^‚ğintŒ^‚É•ÏŠ·
		int n = num[i] - '0';
		// ”š‚É‘Î‰‚·‚é‰æ‘œ‚ğØ‚èo‚·‚½‚ß‚Ì‹éŒ`‚ğŒvZ‚·‚é(”’lƒtƒHƒ“ƒg‰æ‘œ“à‚ÌˆÊ’uj
		GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
		// ”š‚ğ•`‰æ‚·‚éÀ•W‚ğŒvZ
		GSvector2 pos(position.x + i * width_, position.y);
		// ”š‚ğ‚PŒ…•`‰æ
		gsDrawSprite2D(texture_, &pos, &rect, NULL, NULL, NULL, 0);
	}
}