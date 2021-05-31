
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lv_drivers/win_drv.h"
#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"

#include <windows.h>


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#if WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif // WIN32
{
    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();

    /*Check the themes too*/
    lv_disp_set_default(lv_windows_disp);

    /*Run the v7 demo*/
    lv_ui_begin();

#if WIN32
    while(!lv_win_exit_flag)
    {
#else
    while(1)
    {
#endif // WIN32
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(1000);       /*Just to let the system breath*/
    }
    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void)
{
#if !WIN32
    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.disp_flush = monitor_flush;
    disp_drv.disp_fill = monitor_fill;
    disp_drv.disp_map = monitor_map;
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse (or touchpad) as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    /* Tick init.
     * You have to call 'lv_tick_handler()' in every milliseconds
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
#else
    /* This sets up some timers to handle everything. */
    windrv_init();
#endif
}
#if !WIN32
/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data)
{
    while(1)
    {
        lv_tick_inc(1);
        SDL_Delay(1);   /*Sleep for 1 millisecond*/
    }

    return 0;
}
#endif

void lv_ui_begin(void)
{
    init_styles();
    load_boot_screen();

}

//declaration here

LV_IMG_DECLARE(bootlogo2);

//styles here

static lv_style_t bootStyle;
static lv_style_t cbStyle;
static lv_style_t btnStyle;

lv_obj_t * tabs_screen;
lv_obj_t * boot_screen;

//declaration ends

void init_styles(void)
{

    lv_style_init(&bootStyle);
    lv_style_set_bg_color(&bootStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    lv_style_init(&cbStyle);
    lv_style_set_bg_color(&cbStyle, LV_BTN_STATE_PRESSED, LV_COLOR_CYAN);
    lv_style_set_border_color(&cbStyle, LV_BTN_STATE_RELEASED, LV_COLOR_BLUE);
    lv_style_set_border_color(&cbStyle, LV_BTN_STATE_PRESSED, LV_COLOR_TEAL);


    lv_style_set_border_color(&cbStyle, LV_BTN_STATE_CHECKED_PRESSED, LV_COLOR_TEAL);
    lv_style_set_bg_color(&cbStyle, LV_BTN_STATE_CHECKED_PRESSED, LV_COLOR_TEAL);

    lv_style_init(&btnStyle);
    lv_style_set_border_color(&btnStyle, LV_STATE_DEFAULT, LV_COLOR_TEAL);
    lv_style_set_border_color(&btnStyle, LV_STATE_PRESSED, LV_COLOR_TEAL);
    lv_style_set_bg_color(&btnStyle, LV_STATE_PRESSED, LV_COLOR_TEAL);
    lv_style_set_text_color(&btnStyle, LV_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_text_font(&btnStyle, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_style_set_bg_color(&btnStyle, LV_BTN_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_outline_color(&btnStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        printf("Clicked\n");
        load_tabs();
        lv_scr_load_anim(tabs_screen, LV_SCR_LOAD_ANIM_OVER_RIGHT, 300, 100, false);
    }
}

static void event_handler_2(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        printf("Clicked\n");
        load_tabs();
        lv_scr_load_anim(boot_screen, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 100, false);
    }
}

void load_boot_screen(void)
{

    boot_screen = lv_obj_create(NULL, NULL);
    lv_scr_load(boot_screen);
    lv_obj_add_style(lv_scr_act(), LV_OBJ_PART_MAIN, &bootStyle);

    //image
    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &bootlogo2);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, -50);

    //button

    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &btnStyle);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button " LV_SYMBOL_NEXT);

    //checkbox

    lv_obj_t * cb = lv_checkbox_create(lv_scr_act(), NULL);
    lv_checkbox_set_text(cb, "I agree to terms and conditions.");
    lv_obj_align(cb, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -120);
    lv_obj_add_style(cb, LV_CHECKBOX_PART_BULLET, &cbStyle);

    lv_obj_set_style_local_outline_width(cb, LV_STATE_DEFAULT, LV_STATE_DEFAULT, 0);

}

void load_tabs(void)
{

    tabs_screen = lv_obj_create(NULL, NULL);

    lv_obj_t *tabview;
    tabview = lv_tabview_create(tabs_screen, NULL);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");


    /*Add content to the tabs*/
    lv_obj_t * label = lv_label_create(tab1, NULL);
    lv_label_set_text(label, "This the first tab\n\n"
                      "If the content\n"
                      "of a tab\n"
                      "become too long\n"
                      "the it \n"
                      "automatically\n"
                      "become\n"
                      "scrollable.");



    lv_obj_t * btn1 = lv_btn_create(tab1, NULL);
    lv_obj_set_event_cb(btn1, event_handler_2);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, LV_SYMBOL_PREV " BACK");

    label = lv_label_create(tab2, NULL);
    lv_label_set_text(label, "Second tab");

    lv_obj_t * btn2 = lv_btn_create(tab2, NULL);
    lv_obj_align(btn2, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);

    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, LV_SYMBOL_PREV " WTF");


    label = lv_label_create(tab3, NULL);
    lv_label_set_text(label, "Third tab");


}
