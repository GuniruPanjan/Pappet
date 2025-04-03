#include "Font.h"
#include "DxLib.h"

Font::Font():
	m_fontHandle(0)
{
}

Font::~Font()
{
}

void Font::FontInit(int size)
{
	m_fontHandle = AddDxArchiveFontResource("Data/Font/�A�v������.otf", size);
}

int Font::AddDxArchiveFontResource(LPCSTR fontpath, int size)
{
	if (AddFontResourceEx(fontpath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		//�t�H���g�ǂݍ��݃G���[����
		MessageBox(NULL, "�t�H���g�ǂݍ��݃G���[", "", MB_OK);
	}

	return CreateFontToHandle("�A�v������", size, -1, DX_FONTTYPE_NORMAL);
}
