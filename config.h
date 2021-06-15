#define TERMINAL "st"
#define TERMCLASS "St"

#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx      = 3;        /* border pixel of windows */
static const unsigned int snap          = 32;       /* snap pixel */
static const unsigned int gappih        = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv        = 10;       /* vert inner gap between windows */
static const unsigned int gappoh        = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov        = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps              = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar                = 1;        /* 0 means no bar */
static const int topbar                 = 1;        /* 0 means bottom bar */
static const int focusonwheel           = 0;
static const char *fonts[]              = { "Terminus (TTF):size=12","JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]           = "Terminus (TTF):size=12";
static const char normfgcolor[]         = "#bdae93";
static const char selfgcolor[]          = "#fbf1c7";
static const char normbgcolor[]         = "#222222";
static const char selbgcolor[]          = "#9d0006";
static const char normbordercolor[]     = "#282828";
static const char selbordercolor[]      = "#9d0006";
static const unsigned int baralpha      = 0xcd;
static const unsigned int borderalpha   = 0xcd;
static const char *colors[][3]          = {
    /*               fg             bg          border   */
    [SchemeNorm] = { normfgcolor,   normbgcolor,    normbordercolor },
    [SchemeSel]  = { selfgcolor,    selbgcolor, selbordercolor },
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border     */
    [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
    [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i" };

static const char ptagf[] = "[%s %s]";  /* format of a tag label */
static const char etagf[] = "[%s]"; /* format of an empty tag */
static const int lcaselbl = 0;      /* 1 means make tag label lowercase */

static const unsigned int ulinepad      = 5;    /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 0;    /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0;    /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0;    /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
    */
    /* class         instance    title             tags mask       isfloating   isterminal */
    {  TERMCLASS,    NULL,       NULL,             0,              0,           1,         },
    {  NULL,         NULL,       "Event Tester",   0,              0,           0,         },
    {  NULL,         "spterm",   NULL,             SPTAG(0),       1,           1,         },
    {  NULL,         "spcalc",   NULL,             SPTAG(1),       1,           1,         },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "TTT",      bstack },
    { "[@]",      spiral },
    { "[M]",      monocle },
    { "|M|",      centeredmaster },
    //{ "[\\]",     dwindle },
    //{ "H[]",      deck },
    //{ "===",      bstackhoriz },
    //{ "HHH",      grid },
    //{ "###",      nrowgrid },
    //{ "---",      horizgrid },
    //{ ":::",      gaplessgrid },
    //{ ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static Key keys[] = {
    /* modifier             key                         function            argument */
    { MODKEY,               XK_d,                       spawn,              {.v = dmenucmd } },
    { MODKEY,               XK_Return,                  spawn,              {.v = termcmd } },
    { MODKEY|ShiftMask,     XK_Return,                  togglescratch,      {.ui = 0} },
    { MODKEY,               XK_Tab,                     cyclelayout,        {.i = +1 } },
    { MODKEY,               XK_space,                   togglefloating,     {0} },
    { MODKEY,               XK_q,                       killclient,         {0} },
    { MODKEY,               XK_0,                       view,               {.ui = ~0 } },
    { MODKEY,               XK_comma,                   focusmon,           {.i = -1 } },
    { MODKEY,               XK_h,                       setmfact,           {.f = -0.05} },
    { MODKEY,               XK_i,                       incnmaster,         {.i = +1 } },
    { MODKEY,               XK_j,                       focusstack,         {.i = -1 } },
    { MODKEY,               XK_k,                       focusstack,         {.i = +1 } },
    { MODKEY,               XK_l,                       setmfact,           {.f = +0.05} },
    { MODKEY,               XK_period,                  focusmon,           {.i = +1 } },
    { MODKEY,               XK_t,                       togglealttag,       {0} },
    { MODKEY|ControlMask,   XK_b,                       togglebar,          {0} },
    { MODKEY|ShiftMask,     XK_0,                       tag,                {.ui = ~0 } },
    { MODKEY|ShiftMask,     XK_Tab,                     view,               {0} },
    { MODKEY|ShiftMask,     XK_comma,                   tagmon,             {.i = -1 } },
    { MODKEY|ShiftMask,     XK_i,                       incnmaster,         {.i = -1 } },
    { MODKEY|ShiftMask,     XK_j,                       zoom,               {0} },
    { MODKEY|ShiftMask,     XK_k,                       zoom,               {0} },
    { MODKEY|ShiftMask,     XK_period,                  tagmon,             {.i = +1 } },

    { MODKEY,               XK_a,                       spawn,              SHCMD("audacity")},
    { MODKEY,               XK_b,                       spawn,              SHCMD("brave")},
    { MODKEY|ShiftMask,     XK_c,                       spawn,              SHCMD("brave --app-id=ppkkplnhefiifjmgokbhhjebbddhiipf")},
    { MODKEY|ShiftMask,     XK_y,                       spawn,              SHCMD("brave --app-id=agimnkijcaahngcdmfeangaknmldooml")},
    { MODKEY,               XK_f,                       spawn,              SHCMD(TERMINAL " -e lf")},
    { MODKEY,               XK_e,                       spawn,              SHCMD(TERMINAL " -e neomutt")},
    { MODKEY,               XK_g,                       spawn,              SHCMD("gimp")},
    { MODKEY,               XK_m,                       spawn,              SHCMD("musescore")},
    { MODKEY,               XK_r,                       spawn,              SHCMD(TERMINAL " -e newsboat")},
    { MODKEY,               XK_s,                       spawn,              SHCMD("spotifyd && " TERMINAL " -e spt")},
    { MODKEY,               XK_w,                       spawn,              SHCMD(TERMINAL " -e nmtui")},
    { MODKEY,               XK_y,                       spawn,              SHCMD("myyt")},
    { MODKEY|ShiftMask,     XK_b,                       spawn,              SHCMD("qutebrowser --target private-window")},
    { MODKEY|ShiftMask,     XK_h,                       spawn,              SHCMD(TERMINAL " -e htop")},
    { MODKEY|ShiftMask,     XK_f,                       spawn,              SHCMD("pcmanfm")},
    { MODKEY|ShiftMask,     XK_l,                       spawn,              SHCMD("slock -m \"$(cowsay $(fortune) | lolcat -ft)\" & (sleep 5 && xset dpms force off)")},
    { MODKEY|ShiftMask,     XK_p,                       spawn,              SHCMD("poweroff")},
    { MODKEY|ShiftMask,     XK_r,                       spawn,              SHCMD("reboot")},
    { MODKEY,               XK_v,                       spawn,              SHCMD("virt-manager")},

    { 0,                    XF86XK_AudioLowerVolume,    spawn,              SHCMD("pamixer -d 5") },
    { 0,                    XF86XK_AudioMute,           spawn,              SHCMD("pamixer -t") },
    { 0,                    XF86XK_AudioNext,           spawn,              SHCMD("playerctl next") },
    { 0,                    XF86XK_AudioPlay,           spawn,              SHCMD("playerctl play-pause") },
    { 0,                    XF86XK_AudioPrev,           spawn,              SHCMD("playerctl previous") },
    { 0,                    XF86XK_AudioRaiseVolume,    spawn,              SHCMD("pamixer -i 5") },
    { 0,                    XF86XK_MonBrightnessDown,   spawn,              SHCMD("brightnessctl -e s 10%-") },
    { 0,                    XF86XK_MonBrightnessUp,     spawn,              SHCMD("brightnessctl -e s +10%") },
    { 0,                    XK_Print,                   spawn,              SHCMD("scrot -e 'mv $f ~/Pictures/scrots/'") },
    { MODKEY,               XK_F11,                     spawn,              SHCMD("mpv --no-cache --no-osc --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    { MODKEY|ControlMask,   XK_w,                       spawn,              SHCMD("mpv --no-cache --no-osc --profile=low-latency --input-conf=/dev/null --title=whomst ~/Videos/whomst.mp4-QoCcDi8zH8M.mkv") },
    { MODKEY,               XK_c,                       togglescratch,      {.ui = 1} },

    { MODKEY,               XK_bracketleft,             incrgaps,           {.i = +5 } },
    { MODKEY,               XK_bracketright,            incrgaps,           {.i = -5 } },
    { MODKEY,               XK_equal,                   defaultgaps,        {0} },
    TAGKEYS(                XK_1,                       0)
    TAGKEYS(                XK_2,                       1)
    TAGKEYS(                XK_3,                       2)
    TAGKEYS(                XK_4,                       3)
    TAGKEYS(                XK_5,                       4)
    TAGKEYS(                XK_6,                       5)
    TAGKEYS(                XK_7,                       6)
    TAGKEYS(                XK_8,                       7)
    TAGKEYS(                XK_9,                       8)
    { MODKEY|ShiftMask,     XK_q,                       quit,               {0} },
};

/* button definitions */ /* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click            event mask      button          function        argument */
    { ClkLtSymbol,      0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,      0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,      0,              Button2,        zoom,           {0} },
    { ClkStatusText,    0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,     MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,     MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,     MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,        0,              Button1,        view,           {0} },
    { ClkTagBar,        0,              Button3,        toggleview,     {0} },
    { ClkTagBar,        MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,        MODKEY,         Button3,        toggletag,      {0} },
};

