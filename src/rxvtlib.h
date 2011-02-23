#ifndef _RXVTLIB_H_             /* include once only */
#define _RXVTLIB_H_

#include <cstdio>
#include <cctype>
#include <cerrno>
#include <cstdarg>
#include <cstdlib>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
/* #include <util.h> */
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
/* #include <sys/strredir.h> */

#include <sys/wait.h>
#include <sys/stat.h>

#include <cwchar>

#include "ptytty.h"

extern "C" {
#include <X11/Intrinsic.h>      /* Xlib, Xutil, Xresource, Xfuncproto */
}

/*
 * If we haven't pulled in typedef's like  int16_t  then do them ourself
 * type of (normal and unsigned) basic sizes
 */
/* typedef short int16_t; */
/* typedef unsigned short uint16_t; */
/* typedef int int32_t; */
/* typedef unsigned int uint32_t; */

/* whatever normal size corresponds to a integer pointer */
#define intp_t int32_t
/* whatever normal size corresponds to a unsigned integer pointer */
#define u_intp_t u_int32_t

struct rxvt_fontset;
struct rxvt_color;
struct rxvt_vars;               /* defined later on */
struct rxvt_term;
struct rxvt_display;
struct rxvt_im;
struct rxvt_drawable;

typedef struct rxvt_term *rxvt_t;

extern rxvt_t rxvt_current_term;

# define SET_R(r) rxvt_current_term = (r)
# define GET_R rxvt_current_term

#define scrollbar_visible()    scrollBar.state
#define menubar_visible()      menuBar.state

typedef struct {
  int row;
  int col;
} row_col_t;

#if UNICODE_3
typedef uint32_t text_t;
#else
typedef uint16_t text_t; // saves lots of memory
#endif
typedef uint32_t rend_t;

/*
 * TermWin elements limits
 *  width     : 1 <= width
 *  height    : 1 <= height
 *  ncol      : 1 <= ncol       <= MAX(int16_t)
 *  nrow      : 1 <= nrow       <= MAX(int16_t)
 *  saveLines : 0 <= saveLines  <= MAX(int16_t)
 *  nscrolled : 0 <= nscrolled  <= saveLines
 *  view_start: 0 <= view_start <= nscrolled
 */

typedef struct {
  int            width;         /* window width                    [pixels] */
  int            height;        /* window height                   [pixels] */
  int            fwidth;        /* font width                      [pixels] */
  int            fheight;       /* font height                     [pixels] */
  int            fweight, fslant;
  int            fbase;         /* font ascent (baseline)          [pixels] */
  int            ncol;          /* window columns              [characters] */
  int            nrow;          /* window rows                 [characters] */
  int            focus;         /* window has focus                         */
  int            mapped;        /* window state mapped?                     */
  int            int_bwidth;    /* internal border width                    */
  int            ext_bwidth;    /* external border width                    */
  int            lineSpace;     /* number of extra pixels between rows      */
  int            saveLines;     /* number of lines that fit in scrollback   */
  int            nscrolled;     /* number of line actually scrolled         */
  int            view_start;    /* scrollback view starts here              */
  Window         parent[6];     /* parent identifiers - we're parent[0]     */
  Window         vt;            /* vt100 window                             */
  GC             gc;            /* GC for drawing                           */
  Pixmap         pixmap;
  rxvt_drawable *drawable;
  rxvt_fontset  *fontset[4];
} TermWin_t;

/*
 * screen accounting:
 * screen_t elements
 *   text:      Contains all text information including the scrollback buffer.
 *              Each line is length TermWin.ncol
 *   tlen:      The length of the line or -1 for wrapped lines.
 *   rend:      Contains rendition information: font, bold, colour, etc.
 * * Note: Each line for both text and rend are only allocated on demand, and
 *         text[x] is allocated <=> rend[x] is allocated  for all x.
 *   row:       Cursor row position                   : 0 <= row < TermWin.nrow
 *   col:       Cursor column position                : 0 <= col < TermWin.ncol
 *   tscroll:   Scrolling region top row inclusive    : 0 <= row < TermWin.nrow
 *   bscroll:   Scrolling region bottom row inclusive : 0 <= row < TermWin.nrow
 *
 * selection_t elements
 *   clicks:    1, 2 or 3 clicks - 4 indicates a special condition of 1 where
 *              nothing is selected
 *   beg:       row/column of beginning of selection  : never past mark
 *   mark:      row/column of initial click           : never past end
 *   end:       row/column of one character past end of selection
 * * Note: -TermWin.nscrolled <= beg.row <= mark.row <= end.row < TermWin.nrow
 * * Note: col == -1 ==> we're left of screen
 *
 * Layout of text/rend information in the screen_t text/rend structures:
 *   Rows [0] ... [TermWin.saveLines - 1]
 *     scrollback region : we're only here if TermWin.view_start != 0
 *   Rows [TermWin.saveLines] ... [TermWin.saveLines + TermWin.nrow - 1]
 *     normal `unscrolled' screen region
 */
typedef struct {
  int16_t        *tlen; /* length of each text line                  */
  text_t        **text; /* _all_ the text                            */
  rend_t        **rend; /* rendition, uses RS_ flags                 */
  row_col_t       cur;  /* cursor position on the screen             */
  unsigned int    tscroll;      /* top of settable scroll region             */
  unsigned int    bscroll;      /* bottom of settable scroll region          */
  unsigned int    charset;      /* character set number [0..3]               */
  unsigned int    flags;        /* see below                                 */
  row_col_t       s_cur;        /* saved cursor position                     */
  unsigned int    s_charset;    /* saved character set number [0..3]         */
  char            s_charset_char;
  rend_t          s_rstyle;     /* saved rendition style                     */
} screen_t;

enum selection_op_t {
  SELECTION_CLEAR = 0,  /* nothing selected                          */
  SELECTION_INIT,       /* marked a point                            */
  SELECTION_BEGIN,      /* started a selection                       */
  SELECTION_CONT,       /* continued selection                       */
  SELECTION_DONE        /* selection put in CUT_BUFFER0              */
};

typedef struct {
  wchar_t          *text;       /* selected text                             */
  unsigned int      len;        /* length of selected text                   */
  unsigned int      screen;     /* screen being used                         */
  unsigned int      clicks;     /* number of clicks                          */
  selection_op_t    op;         /* current operation                         */
  bool              rect;       /* rectangluar selection?                    */
  row_col_t         beg;        /* beginning of selection   <= mark          */
  row_col_t         mark;       /* point of initial click   <= end           */
  row_col_t         end;        /* one character past end point              */
} selection_t;

/* ------------------------------------------------------------------------- */

/* screen_t flags */
#define Screen_Relative         (1<<0)  /* relative origin mode flag         */
#define Screen_VisibleCursor    (1<<1)  /* cursor visible?                   */
#define Screen_Autowrap         (1<<2)  /* auto-wrap flag                    */
#define Screen_Insert           (1<<3)  /* insert mode (vs. overstrike)      */
#define Screen_WrapNext         (1<<4)  /* need to wrap for next char?       */
#define Screen_DefaultFlags     (Screen_VisibleCursor|Screen_Autowrap)

/* rxvt_vars.options */
#define Opt_console             (1UL<<0)
#define Opt_loginShell          (1UL<<1)
#define Opt_iconic              (1UL<<2)
#define Opt_visualBell          (1UL<<3)
#define Opt_mapAlert            (1UL<<4)
#define Opt_reverseVideo        (1UL<<5)
#define Opt_utmpInhibit         (1UL<<6)
#define Opt_scrollBar           (1UL<<7)
#define Opt_scrollBar_right     (1UL<<8)
#define Opt_scrollBar_floating  (1UL<<9)
#define Opt_meta8               (1UL<<10)
#define Opt_scrollTtyOutput     (1UL<<11)
#define Opt_scrollTtyKeypress   (1UL<<12)
#define Opt_transparent         (1UL<<13)
#define Opt_transparent_all     (1UL<<14)
#define Opt_tripleclickwords    (1UL<<15)
#define Opt_scrollWithBuffer    (1UL<<16)
#define Opt_jumpScroll          (1UL<<17)
#define Opt_mouseWheelScrollPage (1UL<<18)
#define Opt_pointerBlank        (1UL<<19)
#define Opt_cursorBlink         (1UL<<20)
#define Opt_secondaryScreen	(1UL<<21)
#define Opt_secondaryScroll	(1UL<<22)
#if ENABLE_FRILLS
# define Opt_insecure		(1UL<<23) // insecure esc sequences
# define Opt_borderLess		(1UL<<24) // mem borderless hints
#else
# define Opt_insecure		0
# define Opt_borderLess		0
#endif
#define Opt_pastableTabs	(1UL<<25)
/* place holder used for parsing command-line options */
#define Opt_Reverse             (1UL<<30)
#define Opt_Boolean             (1UL<<31)

#define DEFAULT_OPTIONS         (Opt_scrollBar | Opt_scrollTtyOutput \
                                 | Opt_jumpScroll | Opt_secondaryScreen \
                                 | Opt_pastableTabs)

/* ------------------------------------------------------------------------- */

typedef struct {
  short           state;
  Window          win;
  struct rxvt_drawable *drawable;
} menuBar_t;

typedef struct {
  char            state;        /* scrollbar state                          */
  char            init;         /* scrollbar has been initialised           */
  unsigned int    beg;          /* slider sub-window begin height           */
  unsigned int    end;          /* slider sub-window end height             */
  unsigned int    top;          /* slider top position                      */
  unsigned int    bot;          /* slider bottom position                   */
  unsigned int    style;        /* style: rxvt, xterm, next                 */
  unsigned int    width;        /* scrollbar width                          */
  Window          win;
  int             (rxvt_term::*update)(int, int, int, int);

  void setIdle()   { state =  1 ; }
  void setMotion() { state = 'm'; }
  void setUp()     { state = 'U'; }
  void setDn()     { state = 'D'; }
} scrollBar_t;

struct rxvt_vars {
  TermWin_t       TermWin;
  scrollBar_t     scrollBar;
  menuBar_t       menuBar;
  unsigned long   options;
  XSizeHints      szHint;
  rxvt_display   *display;
  rxvt_color     *pix_colors;
  rxvt_color     *pix_colors_focused;
#ifdef OFF_FOCUS_FADING
  rxvt_color     *pix_colors_unfocused;
#endif
  short           numpix_colors;
  Cursor          TermWin_cursor;       /* cursor for vt window */
  int             sb_shadow;    /* scrollbar shadow width                    */
  rxvt_ptytty     pty;
  int             numlock_state;
  text_t        **drawn_text;   /* text drawn on screen (characters)         */
  rend_t        **drawn_rend;   /* text drawn on screen (rendition)          */
  text_t        **buf_text;
  rend_t        **buf_rend;
  char           *tabs;         /* per location: 1 == tab-stop               */
  screen_t        screen;
  screen_t        swap;
  selection_t     selection;
};

void rxvt_init ();

#endif                          /* _RXVTLIB_H_ */

