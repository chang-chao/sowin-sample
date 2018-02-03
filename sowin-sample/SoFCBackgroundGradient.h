#ifndef GUI_SOFCBACKGROUNDGRADIENT_H
#define GUI_SOFCBACKGROUNDGRADIENT_H

#ifndef __InventorAll__
# include "InventorAll.h"
#endif

class SbColor;
class SoGLRenderAction;

class  SoFCBackgroundGradient : public SoNode {
	typedef SoNode inherited;

	SO_NODE_HEADER(SoFCBackgroundGradient);

public:
	static void initClass(void);
	static void finish(void);
	SoFCBackgroundGradient(void);

	void GLRender(SoGLRenderAction *action);
	void setColorGradient(const SbColor& fromColor, const SbColor& toColor);
	void setColorGradient(const SbColor& fromColor, const SbColor& toColor, const SbColor& midColor);

protected:
	virtual ~SoFCBackgroundGradient();

	SbColor fCol, tCol, mCol;
};

#endif // GUI_SOFCBACKGROUNDGRADIENT_H

