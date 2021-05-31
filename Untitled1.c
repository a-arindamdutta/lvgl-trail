#include <stdio.h>
#if LV_USE_SLIDER

static void slider_event_cb(lv_obj_t * slider, lv_event_t event);
static lv_obj_t * slider_label;

void lv_ex_slider_2(void)
{
    /* Create a slider in the center of the display */
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_width(slider, LV_DPI * 2);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(slider, slider_event_cb);
    lv_slider_set_range(slider, 0, 100);

    /* Create a label below the slider */
    slider_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label, "0");
    lv_obj_set_auto_realign(slider_label, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    /* Create an informative label */
    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Welcome to the slider+label demo!\n"
                            "Move the slider and see that the label\n"
                            "updates to match it.");
    lv_obj_align(info, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);
}

static void slider_event_cb(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label, buf);
    }
}

#endif
def event_handler(source,evt):
    if evt == lv.EVENT.VALUE_CHANGED:
        # print(slider.get_value())
        slider_label.set_text(str(slider.get_value()))

# Create a slider in the center of the display
slider = lv.slider(lv.scr_act(),None)
slider.set_width(200)
slider.align(None,lv.ALIGN.CENTER,0,0)
slider.set_event_cb(event_handler)
slider.set_range(0, 100)

# Create a label below the slider
slider_label=lv.label(lv.scr_act(), None)
slider_label.set_text("0")
slider_label.set_auto_realign(True)
slider_label.align(slider,lv.ALIGN.OUT_BOTTOM_MID,0,10)

# Create an informative label
info_label=lv.label(lv.scr_act(), None)
info_label.set_text(
"""Welcome to the slider +
label demo!
Move the slider
and see that the label
updates to match it."""
)
info_label.align(None, lv.ALIGN.IN_TOP_LEFT, 10, 10)
