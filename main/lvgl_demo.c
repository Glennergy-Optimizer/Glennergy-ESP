/**
 * @file lvgl_demo.c
 * @brief Implementation of the LVGL demo UI.
 *
 * @ingroup LVGL_DEMO
 *
 * Builds and loads the demo screen used by the application.
 */

#include "lvgl_demo.h"
#include "gpio.h"
#include "lvgl.h"
#include <math.h>
#include <time.h>

/**
 * @brief Snapshot of demo sensor values used to populate the UI.
 */
typedef struct {
    float temperature; /**< Temperature value. */
    float humidity;    /**< Humidity value. */
    float pressure;    /**< Pressure value. */
    char weather[32];  /**< Weather text. */
} sensor_data_t;

/**
 * @brief Container for 24-point electricity samples.
 */
typedef struct {
    float values[24]; /**< Sample values. */
} electricity_data_t;

/**
 * @brief Container for 24-point forecast samples.
 */
typedef struct {
    float prices[24]; /**< Forecast prices. */
    float temp[24];   /**< Forecast temperatures. */
} forecast_data_t;

/* ================= GLOBAL ================= */

static lv_obj_t * screen;

/* ================= STYLES ================= */

static lv_style_t style_bg, style_card, style_title, style_value;

/**
 * @brief Initializes the shared LVGL styles used by the demo screen.
 */
static void init_styles()
{
    lv_style_init(&style_bg);
    lv_style_set_bg_color(&style_bg, lv_color_hex(0x0F111A));

    lv_style_init(&style_card);
    lv_style_set_bg_color(&style_card, lv_color_hex(0x1C1F2B));
    lv_style_set_radius(&style_card, 14);
    lv_style_set_pad_all(&style_card, 12);
    lv_style_set_border_width(&style_card, 0);

    lv_style_init(&style_title);
    lv_style_set_text_color(&style_title, lv_color_hex(0xA0A3B1));
    lv_style_set_text_font(&style_title, &lv_font_montserrat_14);

    lv_style_init(&style_value);
    lv_style_set_text_color(&style_value, lv_color_white());
    lv_style_set_text_font(&style_value, &lv_font_montserrat_24);
}

/* ================= SIDEBAR ================= */

/**
 * @brief Creates the left-side navigation bar for the demo screen.
 *
 * @param parent Parent LVGL object that receives the sidebar container.
 */
static void create_sidebar(lv_obj_t * parent)
{
    lv_obj_t * sidebar = lv_obj_create(parent);
    lv_obj_set_size(sidebar, 70, LV_PCT(100));
    lv_obj_add_style(sidebar, &style_card, 0);

    lv_obj_set_layout(sidebar, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(sidebar, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(sidebar, 15, 0);

    const char * items[] = {"H", "⚡", "☁", "⚙"};

    for(int i=0;i<4;i++){
        lv_obj_t * btn = lv_btn_create(sidebar);
        lv_obj_set_size(btn, 40, 40);

        lv_obj_t * lbl = lv_label_create(btn);
        lv_label_set_text(lbl, items[i]);
        lv_obj_center(lbl);
    }
}

/* ================= RADIAL ================= */

/**
 * @brief Creates the energy usage card with an arc gauge.
 *
 * @param parent Parent LVGL object that receives the card.
 */
static void create_energy_widget(lv_obj_t * parent)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_add_style(card, &style_card, 0);
    lv_obj_set_size(card, 260, 260);

    lv_obj_t * title = lv_label_create(card);
    lv_label_set_text(title, "Energy Usage");
    lv_obj_add_style(title, &style_title, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * arc = lv_arc_create(card);
    lv_obj_set_size(arc, 180, 180);
    lv_obj_center(arc);

    lv_arc_set_range(arc, 0, 100);
    lv_arc_set_value(arc, 65);

    lv_obj_set_style_arc_width(arc, 14, LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x2A2D3A), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xF5A623), LV_PART_INDICATOR);

    lv_obj_t * val = lv_label_create(card);
    lv_label_set_text(val, "65%");
    lv_obj_add_style(val, &style_value, 0);
    lv_obj_center(val);
}

/* ================= SENSOR CARD ================= */

/**
 * @brief Creates a compact value card for sensor data.
 *
 * @param parent Parent LVGL object that receives the card.
 * @param title_txt Text shown as the card title.
 * @param value_txt Text shown as the card value.
 */
static void create_sensor_card(lv_obj_t * parent, const char * title_txt, const char * value_txt)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_add_style(card, &style_card, 0);
    lv_obj_set_size(card, 140, 80);

    lv_obj_t * t = lv_label_create(card);
    lv_label_set_text(t, title_txt);
    lv_obj_add_style(t, &style_title, 0);

    lv_obj_t * v = lv_label_create(card);
    lv_label_set_text(v, value_txt);
    lv_obj_add_style(v, &style_value, 0);
    lv_obj_align(v, LV_ALIGN_BOTTOM_LEFT, 0, 0);
}

/* ================= GRAPH ================= */

/**
 * @brief Creates a 24-point chart card with random demo values.
 *
 * @param parent Parent LVGL object that receives the chart card.
 * @param title_txt Text shown as the chart title.
 * @param color Series color used for the chart.
 */
static void create_graph(lv_obj_t * parent, const char * title_txt, lv_color_t color)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_add_style(card, &style_card, 0);
    lv_obj_set_size(card, 260, 180);

    lv_obj_t * title = lv_label_create(card);
    lv_label_set_text(title, title_txt);
    lv_obj_add_style(title, &style_title, 0);

    lv_obj_t * chart = lv_chart_create(card);
    lv_obj_set_size(chart, 230, 120);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_chart_set_point_count(chart, 24);
    lv_chart_series_t * ser = lv_chart_add_series(chart, color, LV_CHART_AXIS_PRIMARY_Y);

    for(int i=0;i<24;i++){
        lv_chart_set_next_value(chart, ser, rand()%100);
    }
}

/* ================= MAIN UI ================= */

/**
 * @brief Initializes and loads the LVGL demo screen.
 *
 * Builds the sidebar, summary cards, and charts, then makes the screen active.
 */
void gui_init(void)
{
    init_styles();

    screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_bg, 0);

    /* ROOT GRID */
    static lv_coord_t col_dsc[] = {70, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(screen, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(screen, col_dsc, row_dsc);

    /* SIDEBAR */
    lv_obj_t * sidebar = lv_obj_create(screen);
    lv_obj_set_grid_cell(sidebar, LV_GRID_ALIGN_STRETCH, 0, 1,
                                     LV_GRID_ALIGN_STRETCH, 0, 1);
    create_sidebar(sidebar);

    /* MAIN CONTENT */
    lv_obj_t * main = lv_obj_create(screen);
    lv_obj_set_grid_cell(main, LV_GRID_ALIGN_STRETCH, 1, 1,
                                  LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_obj_set_layout(main, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(main, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(main, 15, 0);

    /* TOP ROW */
    lv_obj_t * top = lv_obj_create(main);
    lv_obj_set_layout(top, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_gap(top, 15, 0);

    create_energy_widget(top);

    lv_obj_t * sensors = lv_obj_create(top);
    lv_obj_set_layout(sensors, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(sensors, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_gap(sensors, 10, 0);

    create_sensor_card(sensors, "Temp", "22.5C");
    create_sensor_card(sensors, "Humidity", "55%");
    create_sensor_card(sensors, "Pressure", "1013hPa");
    create_sensor_card(sensors, "Weather", "Cloudy");

    /* BOTTOM ROW */
    lv_obj_t * bottom = lv_obj_create(main);
    lv_obj_set_layout(bottom, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(bottom, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_gap(bottom, 15, 0);

    create_graph(bottom, "Electricity (0-1)", lv_palette_main(LV_PALETTE_GREEN));
    create_graph(bottom, "Prices 24h", lv_palette_main(LV_PALETTE_RED));

    lv_scr_load(screen);
}
