
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
    //lv_demo_widgets();
    //commenting above because example is not required

    //this function will be used with all display elements
    lv_ui_create();
    //find function below as above is the function call

#if WIN32
    while(!lv_win_exit_flag) {
#else
    while(1) {
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
    while(1) {
        lv_tick_inc(1);
        SDL_Delay(1);   /*Sleep for 1 millisecond*/
    }

    return 0;
}
#endif

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("Selected month: %s\n", buf);
    }
}

static lv_style_t style1;
lv_style_init(&style1);
lv_style_set_border_color(&style1, LV_STATE_FOCUSED, LV_COLOR_BLUE);

void lv_ui_create(void){
    lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(roller1,
                        "January\n"
                        "Feb\n"
                        "March\n"
                        "April\n"
                        "May\n"
                        "June\n"
                        "July\n"
                        "August\n"
                        "September\n"
                        "October\n"
                        "November\n"
                        "December", LV_ROLLER_MODE_INIFINITE
                        );

    lv_roller_set_visible_row_count(roller1, 8);
    lv_obj_add_style(roller1, LV_ROLLER_PART_SELECTED, &style1);
    lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, 0 , 0 );
    lv_obj_set_event_cb(roller1, event_handler);
    lv_roller_set_selected(roller1, 9 , LV_ANIM_ON);

    //the above line sets an event handler function which is called whenever the widgets has touch inputs.
    //we need to write that event-handler function too
}



