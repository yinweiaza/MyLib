#pragma once
#include "GdiPlusInclude.h"

//用到GDI+绘图的一些函数
inline HRESULT DrawScrnTableCloth(	LPBYTE	lpBuf,
								  int		width, 
								  int		height, 
								  LPCTSTR strName, 
								  LPCTSTR	lpFile)
{
	if(lpBuf == NULL || width <= 0 || height<=0 || lpFile == NULL)
		return T9E_ParamError;

	Bitmap bitmap(width, height, PixelFormat32bppARGB);
	Graphics graphics(&bitmap);

	Pointer<Image> pImage = Image::FromFile(CT2W(lpFile));
	Status status = pImage->GetLastStatus();
	if(status != Ok)
		return T9E_OpenFileError;


	//图像的大小
	int src_width = pImage->GetWidth();
	int src_height = pImage->GetHeight();

	int xRepeat = width/src_width;
	if(xRepeat*src_width<width)
		xRepeat++;

	int yRepeat = height/src_height;
	if(yRepeat*src_height<height)
		yRepeat++;

	for(int x=0; x < xRepeat; x++)
	{
		for(int y=0; y<yRepeat;y++)
		{
			graphics.DrawImage(pImage,x*src_width,y*src_height,src_width,src_height);   //把图像写入到内存,采取和显示区域一样的大小.
		}
	}

	HDC hdc = graphics.GetHDC();
	CDC cdc;
	cdc.Attach(hdc);

	CString str;
	RECT rc;
	str = strName;
	// 		.Format("LED %d",scrnNumber);
	int len = str.GetLength();

	cdc.SetTextColor(RGB(255,255,255));
	cdc.SetBkColor(RGB(0,0,0));
	cdc.SetBkMode(TRANSPARENT );

	CRect tmrc;
	cdc.DrawText(str,len,&tmrc,DT_CALCRECT);
	SetRect( &rc, 10, 0, 10+tmrc.Width(), tmrc.Height() );
	cdc.DrawText(str,len,&rc,DT_SINGLELINE|DT_LEFT);
	SetRect( &rc, width - tmrc.Width()-10, 0, width - 10, tmrc.Height()  );
	cdc.DrawText( str, len, &rc,DT_SINGLELINE|DT_RIGHT);
	SetRect( &rc, 10, height - tmrc.Height(), 10+tmrc.Width(),height );
	cdc.DrawText(str, len, &rc,DT_SINGLELINE|DT_LEFT );
	SetRect( &rc, width - tmrc.Width()-10, height - tmrc.Height(), width - 10, height );
	cdc.DrawText( str, len, &rc,DT_SINGLELINE|DT_RIGHT);
	SetRect( &rc, (width-tmrc.Width())/2, (height-tmrc.Height())/2, (width+tmrc.Width())/2, (height+tmrc.Height())/2);
	cdc.DrawText(str, len, &rc,DT_SINGLELINE|DT_CENTER);

	cdc.Detach();
	graphics.ReleaseHDC(hdc);

	BitmapData bmData  = {0};
	Rect rcCrop(0, 0, width, height);
	Status sts = bitmap.LockBits(	&rcCrop,
		ImageLockModeRead, 
		PixelFormat32bppRGB,
		&bmData);

	if(sts != Ok)
		return  E_FAIL;

	memcpy(lpBuf, bmData.Scan0, width*height*4);

	bitmap.UnlockBits(&bmData);

	return T9_NOERROR;
}