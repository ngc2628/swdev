
#include <osix/xxevent.h>
#include <QtGui/QKeyEvent>

namespace qtutil {

int translateQkey(QKeyEvent *qk,osix::xxEvent *xk) {

  xk->m_xxk=0;
  Qt::Key qkey=(Qt::Key)qk->key();
  if (qkey==Qt::Key_Escape)
    xk->m_xxk=osix::xxk_Escape;
  else if (qkey==Qt::Key_Tab)
    xk->m_xxk=osix::xxk_Tab;
  else if (qkey==Qt::Key_Backtab)
    xk->m_xxk=osix::xxk_BackTab;
  else if (qkey==Qt::Key_Backspace)
    xk->m_xxk=osix::xxk_BackSpace;
  else if (qkey==Qt::Key_Return)
    xk->m_xxk=osix::xxk_Return;
  else if (qkey==Qt::Key_Enter)
    xk->m_xxk=osix::xxk_Enter;
  else if (qkey==Qt::Key_Insert)
    xk->m_xxk=osix::xxk_Insert;
  else if (qkey==Qt::Key_Delete)
    xk->m_xxk=osix::xxk_Delete;
  else if (qkey==Qt::Key_Pause)
    xk->m_xxk=osix::xxk_Pause;
  else if (qkey==Qt::Key_Print)
    xk->m_xxk=osix::xxk_Print;
  else if (qkey==Qt::Key_Left)
    xk->m_xxk=osix::xxk_Left;
  else if (qkey==Qt::Key_Right)
    xk->m_xxk=osix::xxk_Right;
  else if (qkey==Qt::Key_Up)
    xk->m_xxk=osix::xxk_Up;
  else if (qkey==Qt::Key_Down)
    xk->m_xxk=osix::xxk_Down;
  else if (qkey==Qt::Key_Home)
    xk->m_xxk=osix::xxk_Home;
  else if (qkey==Qt::Key_PageUp)
    xk->m_xxk=osix::xxk_Page_Up;
  else if (qkey==Qt::Key_PageDown)
    xk->m_xxk=osix::xxk_Page_Down;
  else if (qkey==Qt::Key_Shift)
    xk->m_xxk=osix::xxk_Shift;
  else if (qkey==Qt::Key_Control)
    xk->m_xxk=osix::xxk_Control;
  else if (qkey==Qt::Key_Meta)
    xk->m_xxk=osix::xxk_Meta;
  else if (qkey==Qt::Key_Alt)
    xk->m_xxk=osix::xxk_Alt;
  else if (qkey==Qt::Key_AltGr)
    xk->m_xxk=osix::xxk_Group_Shift;
  else if (qkey==Qt::Key_CapsLock)
    xk->m_xxk=osix::xxk_Caps_Lock;
  else if (qkey==Qt::Key_NumLock)
    xk->m_xxk=osix::xxk_Num_Lock;
  else if (qkey==Qt::Key_ScrollLock)
    xk->m_xxk=osix::xxk_Scroll_Lock;
  else if (qkey==Qt::Key_F1)
    xk->m_xxk=osix::xxk_F1;
  else if (qkey==Qt::Key_F2)
    xk->m_xxk=osix::xxk_F2;
  else if (qkey==Qt::Key_F3)
    xk->m_xxk=osix::xxk_F3;
  else if (qkey==Qt::Key_F4)
    xk->m_xxk=osix::xxk_F4;
  else if (qkey==Qt::Key_F5)
    xk->m_xxk=osix::xxk_F5;
  else if (qkey==Qt::Key_F6)
    xk->m_xxk=osix::xxk_F6;
  else if (qkey==Qt::Key_F7)
    xk->m_xxk=osix::xxk_F7;
  else if (qkey==Qt::Key_F8)
    xk->m_xxk=osix::xxk_F8;
  else if (qkey==Qt::Key_F9)
    xk->m_xxk=osix::xxk_F9;
  else if (qkey==Qt::Key_F10)
    xk->m_xxk=osix::xxk_F10;
  else if (qkey==Qt::Key_F11)
    xk->m_xxk=osix::xxk_F11;
  else if (qkey==Qt::Key_F12) 
    xk->m_xxk=osix::xxk_F12;
  else if (qkey==Qt::Key_F13)
    xk->m_xxk=osix::xxk_F13;
  else if (qkey==Qt::Key_F14)
    xk->m_xxk=osix::xxk_F14;
  else if (qkey==Qt::Key_F15)
    xk->m_xxk=osix::xxk_F15;
  else if (qkey==Qt::Key_Super_L)
    xk->m_xxk=osix::xxk_Super_L;
  else if (qkey==Qt::Key_Super_R)
    xk->m_xxk=osix::xxk_Super_R;
  else if (qkey==Qt::Key_Hyper_L)
    xk->m_xxk=osix::xxk_Hyper_L;
  else if (qkey==Qt::Key_Hyper_R)
    xk->m_xxk=osix::xxk_Hyper_R;
  else if (qkey==Qt::Key_Direction_L)
    xk->m_xxk=osix::xxk_Left;
  else if (qkey==Qt::Key_Direction_R)
    xk->m_xxk=osix::xxk_Right;
  else if (qkey==Qt::Key_Space)
    xk->m_xxk=osix::xxk_space;
  else if (qkey==Qt::Key_Exclam)
    xk->m_xxk=osix::xxk_exclam;
  else if (qkey==Qt::Key_QuoteDbl)
    xk->m_xxk=osix::xxk_quotedbl;
  else if (qkey==Qt::Key_NumberSign)
    xk->m_xxk=osix::xxk_numbersign;
  else if (qkey==Qt::Key_Dollar)
    xk->m_xxk=osix::xxk_dollar;
  else if (qkey==Qt::Key_Percent)
    xk->m_xxk=osix::xxk_percent;
  else if (qkey==Qt::Key_Ampersand)
    xk->m_xxk=osix::xxk_ampersand;
  else if (qkey==Qt::Key_Apostrophe)
    xk->m_xxk=osix::xxk_apostrophe;
  else if (qkey==Qt::Key_ParenLeft)
    xk->m_xxk=osix::xxk_parenleft;
  else if (qkey==Qt::Key_ParenRight)
    xk->m_xxk=osix::xxk_parenright;
  else if (qkey==Qt::Key_Asterisk)
    xk->m_xxk=osix::xxk_asterisk;
  else if (qkey==Qt::Key_Plus)
    xk->m_xxk=osix::xxk_plus;
  else if (qkey==Qt::Key_Comma)
    xk->m_xxk=osix::xxk_comma;
  else if (qkey==Qt::Key_Minus)
    xk->m_xxk=osix::xxk_minus;
  else if (qkey==Qt::Key_Period)
    xk->m_xxk=osix::xxk_period;
  else if (qkey==Qt::Key_Slash)
    xk->m_xxk=osix::xxk_slash;
  else if (qkey==Qt::Key_0)
    xk->m_xxk=osix::xxk_0;
  else if (qkey==Qt::Key_1)
    xk->m_xxk=osix::xxk_1;
  else if (qkey==Qt::Key_2)
    xk->m_xxk=osix::xxk_2;
  else if (qkey==Qt::Key_3)
    xk->m_xxk=osix::xxk_3;
  else if (qkey==Qt::Key_4)
    xk->m_xxk=osix::xxk_4;
  else if (qkey==Qt::Key_5)
    xk->m_xxk=osix::xxk_5;
  else if (qkey==Qt::Key_6)
    xk->m_xxk=osix::xxk_6;
  else if (qkey==Qt::Key_7)
    xk->m_xxk=osix::xxk_7;
  else if (qkey==Qt::Key_8)
    xk->m_xxk=osix::xxk_8;
  else if (qkey==Qt::Key_9)
    xk->m_xxk=osix::xxk_9;
  else if (qkey==Qt::Key_Colon)
    xk->m_xxk=osix::xxk_colon;
  else if (qkey==Qt::Key_Semicolon)
    xk->m_xxk=osix::xxk_semicolon;
  else if (qkey==Qt::Key_Less)
    xk->m_xxk=osix::xxk_less;
  else if (qkey==Qt::Key_Equal)
    xk->m_xxk=osix::xxk_equal;
  else if (qkey==Qt::Key_Greater)
    xk->m_xxk=osix::xxk_greater;
  else if (qkey==Qt::Key_Question)
    xk->m_xxk=osix::xxk_question;
  else if (qkey==Qt::Key_At)
    xk->m_xxk=osix::xxk_at;
  else if (qkey==Qt::Key_A)
    xk->m_xxk=osix::xxk_A;
  else if (qkey==Qt::Key_B)
    xk->m_xxk=osix::xxk_B;
  else if (qkey==Qt::Key_C)
    xk->m_xxk=osix::xxk_C;
  else if (qkey==Qt::Key_D)
    xk->m_xxk=osix::xxk_D;
  else if (qkey==Qt::Key_E)
    xk->m_xxk=osix::xxk_E;
  else if (qkey==Qt::Key_F)
    xk->m_xxk=osix::xxk_F;
  else if (qkey==Qt::Key_G)
    xk->m_xxk=osix::xxk_G;
  else if (qkey==Qt::Key_H)
    xk->m_xxk=osix::xxk_H;
  else if (qkey==Qt::Key_I)
    xk->m_xxk=osix::xxk_I;
  else if (qkey==Qt::Key_J)
    xk->m_xxk=osix::xxk_J;
  else if (qkey==Qt::Key_K)
    xk->m_xxk=osix::xxk_K;
  else if (qkey==Qt::Key_L)
    xk->m_xxk=osix::xxk_L;
  else if (qkey==Qt::Key_M)
    xk->m_xxk=osix::xxk_M;
  else if (qkey==Qt::Key_N)
    xk->m_xxk=osix::xxk_N;
  else if (qkey==Qt::Key_O)
    xk->m_xxk=osix::xxk_O;
  else if (qkey==Qt::Key_P)
    xk->m_xxk=osix::xxk_P;
  else if (qkey==Qt::Key_Q)
    xk->m_xxk=osix::xxk_Q;
  else if (qkey==Qt::Key_R)
    xk->m_xxk=osix::xxk_R;
  else if (qkey==Qt::Key_S)
    xk->m_xxk=osix::xxk_S;
  else if (qkey==Qt::Key_T)
    xk->m_xxk=osix::xxk_T;
  else if (qkey==Qt::Key_U)
    xk->m_xxk=osix::xxk_U;
  else if (qkey==Qt::Key_V)
    xk->m_xxk=osix::xxk_V;
  else if (qkey==Qt::Key_W)
    xk->m_xxk=osix::xxk_W;
  else if (qkey==Qt::Key_X)
    xk->m_xxk=osix::xxk_X;
  else if (qkey==Qt::Key_Y)
    xk->m_xxk=osix::xxk_Y;
  else if (qkey==Qt::Key_Z)
    xk->m_xxk=osix::xxk_Z;

  if (qkey==Qt::Key_BracketLeft)
    xk->m_xxk=osix::xxk_bracketleft;
  else if (qkey==Qt::Key_Backslash)
    xk->m_xxk=osix::xxk_backslash;
  else if (qkey==Qt::Key_BracketRight)
    xk->m_xxk=osix::xxk_bracketright;
  else if (qkey==Qt::Key_AsciiCircum)
    xk->m_xxk=osix::xxk_asciicircum;
  else if (qkey==Qt::Key_Underscore)
    xk->m_xxk=osix::xxk_underscore;
  else if (qkey==Qt::Key_QuoteLeft)
    xk->m_xxk=osix::xxk_quoteleft;
  else if (qkey==Qt::Key_BraceLeft)
    xk->m_xxk=osix::xxk_braceleft;
  else if (qkey==Qt::Key_Bar)
    xk->m_xxk=osix::xxk_bar;
  else if (qkey==Qt::Key_BraceRight)
    xk->m_xxk=osix::xxk_braceright;
  else if (qkey==Qt::Key_AsciiTilde)
    xk->m_xxk=osix::xxk_asciitilde;
  else if (qkey==Qt::Key_nobreakspace)
    xk->m_xxk=osix::xxk_nobreakspace;
  else if (qkey==Qt::Key_exclamdown)
    xk->m_xxk=osix::xxk_exclamdown;
  else if (qkey==Qt::Key_cent)
    xk->m_xxk=osix::xxk_cent;
  else if (qkey==Qt::Key_sterling)
    xk->m_xxk=osix::xxk_sterling;
  else if (qkey==Qt::Key_currency)
    xk->m_xxk=osix::xxk_currency;
  else if (qkey==Qt::Key_yen)
    xk->m_xxk=osix::xxk_yen;
  else if (qkey==Qt::Key_brokenbar)
    xk->m_xxk=osix::xxk_brokenbar;
  else if (qkey==Qt::Key_section)
    xk->m_xxk=osix::xxk_section;
  else if (qkey==Qt::Key_diaeresis)
    xk->m_xxk=osix::xxk_diaeresis;
  else if (qkey==Qt::Key_copyright)
    xk->m_xxk=osix::xxk_copyright;
  else if (qkey==Qt::Key_ordfeminine)
    xk->m_xxk=osix::xxk_ordfeminine;
  else if (qkey==Qt::Key_guillemotleft)
    xk->m_xxk=osix::xxk_guillemotleft;
  else if (qkey==Qt::Key_notsign)
    xk->m_xxk=osix::xxk_notsign;
  else if (qkey==Qt::Key_hyphen)
    xk->m_xxk=osix::xxk_hyphen;
  else if (qkey==Qt::Key_registered)
    xk->m_xxk=osix::xxk_registered;
  else if (qkey==Qt::Key_macron)
    xk->m_xxk=osix::xxk_macron;
  else if (qkey==Qt::Key_degree)
    xk->m_xxk=osix::xxk_degree;
  else if (qkey==Qt::Key_plusminus)
    xk->m_xxk=osix::xxk_plusminus;
  else if (qkey==Qt::Key_twosuperior)
    xk->m_xxk=osix::xxk_twosuperior;
  else if (qkey==Qt::Key_threesuperior)
    xk->m_xxk=osix::xxk_threesuperior;
  else if (qkey==Qt::Key_acute)
    xk->m_xxk=osix::xxk_acute;
  else if (qkey==Qt::Key_mu)
    xk->m_xxk=osix::xxk_mu;
  else if (qkey==Qt::Key_paragraph)
    xk->m_xxk=osix::xxk_paragraph;
  else if (qkey==Qt::Key_periodcentered)
    xk->m_xxk=osix::xxk_periodcentered;
  else if (qkey==Qt::Key_cedilla)
    xk->m_xxk=osix::xxk_cedilla;
  else if (qkey==Qt::Key_onesuperior)
    xk->m_xxk=osix::xxk_onesuperior;
  else if (qkey==Qt::Key_masculine)
    xk->m_xxk=osix::xxk_masculine;
  else if (qkey==Qt::Key_guillemotright)
    xk->m_xxk=osix::xxk_guillemotright;
  else if (qkey==Qt::Key_onequarter)
    xk->m_xxk=osix::xxk_onequarter;
  else if (qkey==Qt::Key_onehalf)
    xk->m_xxk=osix::xxk_onehalf;
  else if (qkey==Qt::Key_threequarters)
    xk->m_xxk=osix::xxk_threequarters;
  else if (qkey==Qt::Key_questiondown)
    xk->m_xxk=osix::xxk_questiondown;
  else if (qkey==Qt::Key_Agrave)
    xk->m_xxk=osix::xxk_Agrave;
  else if (qkey==Qt::Key_Aacute)
    xk->m_xxk=osix::xxk_Aacute;
  else if (qkey==Qt::Key_Acircumflex)
    xk->m_xxk=osix::xxk_Acircumflex;
  else if (qkey==Qt::Key_Atilde)
    xk->m_xxk=osix::xxk_Atilde;
  else if (qkey==Qt::Key_Adiaeresis)
    xk->m_xxk=osix::xxk_Adiaeresis;
  else if (qkey==Qt::Key_Aring)
    xk->m_xxk=osix::xxk_Aring;
  else if (qkey==Qt::Key_AE)
    xk->m_xxk=osix::xxk_AE;
  else if (qkey==Qt::Key_Ccedilla)
    xk->m_xxk=osix::xxk_Ccedilla;
  else if (qkey==Qt::Key_Egrave)
    xk->m_xxk=osix::xxk_Egrave;
  else if (qkey==Qt::Key_Eacute)
    xk->m_xxk=osix::xxk_Eacute;
  else if (qkey==Qt::Key_Ecircumflex)
    xk->m_xxk=osix::xxk_Ecircumflex;
  else if (qkey==Qt::Key_Ediaeresis)
    xk->m_xxk=osix::xxk_Ediaeresis;
  else if (qkey==Qt::Key_Igrave)
    xk->m_xxk=osix::xxk_Igrave;
  else if (qkey==Qt::Key_Iacute)
    xk->m_xxk=osix::xxk_Iacute;
  else if (qkey==Qt::Key_Icircumflex)
    xk->m_xxk=osix::xxk_Icircumflex;
  else if (qkey==Qt::Key_Idiaeresis)
    xk->m_xxk=osix::xxk_Idiaeresis;
  else if (qkey==Qt::Key_ETH)
    xk->m_xxk=osix::xxk_ETH;
  else if (qkey==Qt::Key_Ntilde)
    xk->m_xxk=osix::xxk_Ntilde;
  else if (qkey==Qt::Key_Ograve)
    xk->m_xxk=osix::xxk_Ograve;
  else if (qkey==Qt::Key_Oacute)
    xk->m_xxk=osix::xxk_Oacute;
  else if (qkey==Qt::Key_Ocircumflex)
    xk->m_xxk=osix::xxk_Ocircumflex;
  else if (qkey==Qt::Key_Otilde)
    xk->m_xxk=osix::xxk_Otilde;
  else if (qkey==Qt::Key_Odiaeresis)
    xk->m_xxk=osix::xxk_Odiaeresis;
  else if (qkey==Qt::Key_multiply)
    xk->m_xxk=osix::xxk_multiply;
  else if (qkey==Qt::Key_Ooblique)
    xk->m_xxk=osix::xxk_Ooblique;
  else if (qkey==Qt::Key_Ugrave)
    xk->m_xxk=osix::xxk_Ugrave;
  else if (qkey==Qt::Key_Uacute)
    xk->m_xxk=osix::xxk_Uacute;
  else if (qkey==Qt::Key_Ucircumflex)
    xk->m_xxk=osix::xxk_Ucircumflex;
  else if (qkey==Qt::Key_Udiaeresis)
    xk->m_xxk=osix::xxk_Udiaeresis;
  else if (qkey==Qt::Key_Yacute)
    xk->m_xxk=osix::xxk_Yacute;
  else if (qkey==Qt::Key_THORN)
    xk->m_xxk=osix::xxk_THORN;
  else if (qkey==Qt::Key_ydiaeresis)
    xk->m_xxk=osix::xxk_ydiaeresis;
  else if (qkey==Qt::Key_division)
    xk->m_xxk=osix::xxk_division;
  else if (qkey==Qt::Key_ssharp)
    xk->m_xxk=osix::xxk_ssharp;
  
  //printf ("%d mod=%d\n",__LINE__,(int)qk->modifiers());

  if (((int)qk->modifiers()&(int)Qt::ShiftModifier)>0)
    xk->m_mods|=osix::xx_modShift;
  if (((int)qk->modifiers()&(int)Qt::ControlModifier)>0)
    xk->m_mods|=osix::xx_modCtrl;
  if (((int)qk->modifiers()&(int)Qt::AltModifier)>0)
    xk->m_mods|=osix::xx_modAlt;
  if (((int)qk->modifiers()&(int)Qt::MetaModifier)>0)
    xk->m_mods|=osix::xx_modMeta;
  if (((int)qk->modifiers()&(int)Qt::KeypadModifier)>0)
    xk->m_mods|=osix::xx_modKeypad;
  if (((int)qk->modifiers()&(int)Qt::GroupSwitchModifier)>0)
    xk->m_mods|=osix::xx_modGroupSwitch;

  const unsigned short *strutf16=qk->text().utf16();
  if (strutf16)
    xk->m_xxsym=strutf16[0];

  return 0;

}

}
