// -*- mode: C++; indent-tabs-mode: nil; c-basic-offset: 2; -*-

#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif

#include "focuslabel.hh"
#include "display.hh"
#include "screeninfo.hh"

namespace otk {

OtkFocusLabel::OtkFocusLabel(OtkWidget *parent)
  : OtkFocusWidget(parent), _text("")
{
  const ScreenInfo *info = OBDisplay::screenInfo(getScreen());
  _xftdraw = XftDrawCreate(OBDisplay::display, getWindow(), info->getVisual(),
                           info->getColormap());
  setStyle(getStyle());
}

OtkFocusLabel::~OtkFocusLabel()
{
  XftDrawDestroy(_xftdraw);
}


void OtkFocusLabel::setStyle(Style *style)
{
  OtkFocusWidget::setStyle(style);
  
  setTexture(getStyle()->getLabelFocus());
  setUnfocusTexture(getStyle()->getLabelUnfocus());
}


void OtkFocusLabel::update(void)
{
  if (_dirty) {
    const BFont &ft = getStyle()->getFont();
    BColor *text_color = (isFocused() ? getStyle()->getTextFocus()
                          : getStyle()->getTextUnfocus());
    unsigned int sidemargin = getStyle()->getBevelWidth() * 2;

    std::string t = _text; // the actual text to draw
    int x = sidemargin;    // x coord for the text

    // find a string that will fit inside the area for text
    int max_length = width() - sidemargin * 2;
    if (max_length <= 0) {
      t = ""; // can't fit anything
    } else {
      size_t text_len = t.size();
      int length;
      
      do {
        t.resize(text_len);
        length = ft.measureString(t);
      } while (length > max_length && text_len-- > 0);

      // justify the text
      switch (getStyle()->textJustify()) {
      case Style::RightJustify:
        x += max_length - length;
        break;
      case Style::CenterJustify:
        x += (max_length - length) / 2;
        break;
      case Style::LeftJustify:
        break;
      }
    }

    OtkFocusWidget::update();

    ft.drawString(_xftdraw, x, 0, *text_color, t);
  } else
    OtkFocusWidget::update();
}

}
