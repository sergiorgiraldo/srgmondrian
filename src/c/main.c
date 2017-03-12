/* ---------------------------------------------------------
    Mondrian
    Copyright (C) 2015 Owen Bullock 
                       owen.skimapp@gmail.com
                       
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ---------------------------------------------------------*/
#include <pebble.h>
  
   
static void update_time();

static Window      *s_main_window;
static TextLayer   *s_time_layer,
                   *s_date_layer;
       Layer       *s_graph_layer;
static BitmapLayer *s_bt_icon_layer;
static GBitmap     *s_bt_icon_bitmap;


#define COL_HOURS GColorBlue
#define COL_MINS  GColorYellow
#define COL_BOTH  GColorRed
#define COL_NONE  GColorWhite
    

static int fill_h[]  = {0,0,0,0,0};   // 5,3,2,1,1
static int fill_m[] = {0,0,0,0,0};

       void gfx_layer_update_callback(Layer *me, GContext* ctx);
static void fill_rect_5(GContext* ctx, int fill_hrs, int fill_mins);
static void fill_rect_3(GContext* ctx, int fill_hrs, int fill_mins);
static void fill_rect_2(GContext* ctx, int fill_hrs, int fill_mins);
static void fill_rect_1a(GContext* ctx, int fill_hrs, int fill_mins);
static void fill_rect_1b(GContext* ctx, int fill_hrs, int fill_mins);

static int do_bt_vibe = 0; // first time, including when switching back from timeline

static void bluetooth_callback(bool connected) {
  // Show icon if disconnected
  layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), connected);

  // Issue a vibrating alert
  if(!connected) {
      vibes_double_pulse();
  } else {
      if (do_bt_vibe)  // dont vibe if BT enabled and switching back from timeline
          vibes_short_pulse();
  }
  
  do_bt_vibe = 1;
}
  

static void mainwindow_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 118, 144, 50));  // was 55 - centre
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  //text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  
  //   date   
  s_date_layer = text_layer_create(GRect(5, 100, 139, 30));
  text_layer_set_font(s_date_layer,   fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text(s_date_layer, "Today 10");
  
  
  // graphics layer
  s_graph_layer = layer_create(GRect(0, 0, 144, 168));
  
  // BT indicator
  s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ICON);
  s_bt_icon_layer = bitmap_layer_create(GRect(0, 131, 30, 30));
  bitmap_layer_set_compositing_mode(s_bt_icon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);


  //callback
  layer_set_update_proc(s_graph_layer, gfx_layer_update_callback);

  
  // add to parent
  layer_add_child(window_get_root_layer(window), s_graph_layer);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bt_icon_layer));

  
  // set all to white
  window_set_background_color(s_main_window, GColorWhite);
  
  // Show the correct state of the BT connection from the start
  bluetooth_callback(bluetooth_connection_service_peek());

}


void gfx_layer_update_callback(Layer *me, GContext* ctx) {
//   APP_LOG(APP_LOG_LEVEL_INFO, "gfx_layer_update_callback() - entry" );

  // Colour in boxes
  fill_rect_5  (ctx, fill_h[0], fill_m[0]);
  fill_rect_3  (ctx, fill_h[1], fill_m[1]);
  fill_rect_2  (ctx, fill_h[2], fill_m[2]);
  fill_rect_1a (ctx, fill_h[3], fill_m[3]);
  fill_rect_1b (ctx, fill_h[4], fill_m[4]);
 
  
    // draw grid lines
  graphics_context_set_stroke_color(ctx, GColorBlack);
    // A
  graphics_draw_line(ctx, (GPoint){0  , 83},
                          (GPoint){144, 83} );
  graphics_draw_line(ctx, (GPoint){0  , 84},
                          (GPoint){144, 84} );
  graphics_draw_line(ctx, (GPoint){0  , 85},
                          (GPoint){144, 85} );
  graphics_draw_line(ctx, (GPoint){0  , 86},
                          (GPoint){144, 86} );
    // B
  graphics_draw_line(ctx, (GPoint){71, 0},
                          (GPoint){71, 84} );
  graphics_draw_line(ctx, (GPoint){72, 0},
                          (GPoint){72, 84} );
  graphics_draw_line(ctx, (GPoint){73, 0},
                          (GPoint){73, 84} );
  graphics_draw_line(ctx, (GPoint){74, 0},
                          (GPoint){74, 84} );
    // C
  graphics_draw_line(ctx, (GPoint){72,  41},
                          (GPoint){144, 41} );
  graphics_draw_line(ctx, (GPoint){72,  42},
                          (GPoint){144, 42} );
  graphics_draw_line(ctx, (GPoint){72,  43},
                          (GPoint){144, 43} );
  graphics_draw_line(ctx, (GPoint){72,  44},
                          (GPoint){144, 44} );
    // D
  graphics_draw_line(ctx, (GPoint){107, 0},
                          (GPoint){107, 42} );
  graphics_draw_line(ctx, (GPoint){108, 0},
                          (GPoint){108, 42} );
  graphics_draw_line(ctx, (GPoint){109, 0},
                          (GPoint){109, 42} );
  graphics_draw_line(ctx, (GPoint){110, 0},
                          (GPoint){110, 42} );
  


  
//   APP_LOG(APP_LOG_LEVEL_INFO, "gfx_layer_update_callback() - exit" );
}



static void set_colour(GContext* ctx, int fill_hrs, int fill_mins)
{
  GColor                      col = COL_NONE;
  
//   APP_LOG(APP_LOG_LEVEL_INFO, "set_colour");

  if (fill_hrs)               {col = COL_HOURS;
                               APP_LOG(APP_LOG_LEVEL_INFO, "  set_colour  Blue");}
  if (fill_mins)              {col = COL_MINS;
                               APP_LOG(APP_LOG_LEVEL_INFO, "  set_colour  Yellow");}
  if (fill_hrs && fill_mins)  {col = COL_BOTH;
                               APP_LOG(APP_LOG_LEVEL_INFO, "  set_colour  Red");}
  
  graphics_context_set_fill_color(ctx, col);
//     APP_LOG(APP_LOG_LEVEL_INFO, "/set_colour");

}

static void set_colour_5(GContext* ctx, int fill_hrs, int fill_mins)
{
  GColor                      col = COL_NONE;
  
   APP_LOG(APP_LOG_LEVEL_INFO, "set_colour_5");

  text_layer_set_text_color(s_time_layer, GColorBlack);

   if (fill_hrs)               { col = COL_HOURS;
                                 text_layer_set_text_color(s_date_layer, GColorWhite);
                                 text_layer_set_text_color(s_time_layer, GColorWhite); }
   if (fill_mins)              { col = COL_MINS;
                                 APP_LOG(APP_LOG_LEVEL_INFO, "  set_colour  Yellow");
                                text_layer_set_text_color(s_date_layer, GColorBlack);
                                 text_layer_set_text_color(s_time_layer, GColorBlack);}
   if (fill_hrs && fill_mins)  { col = COL_BOTH;
                                 text_layer_set_text_color(s_date_layer, GColorWhite);
                                 text_layer_set_text_color(s_time_layer, GColorWhite);}
  

  graphics_context_set_fill_color(ctx, col);
//     APP_LOG(APP_LOG_LEVEL_INFO, "/set_colour_5");

 

}


//  rectangles + colour logic
static void fill_rect_5(GContext* ctx, int fill_hrs, int fill_mins)
{
//   APP_LOG(APP_LOG_LEVEL_INFO, "fill_rect_5");

  set_colour_5(ctx, fill_hrs, fill_mins);
  
  graphics_fill_rect	(	 	ctx, (GRect) {.origin = { 0   , 84 }, 
                                        .size   = { 144 , 84 }}, 0, 0);
}
static void fill_rect_3(GContext* ctx, int fill_hrs, int fill_mins)
{
//     APP_LOG(APP_LOG_LEVEL_INFO, "fill_rect_3");

  set_colour(ctx, fill_hrs, fill_mins);
  graphics_fill_rect	(	 	ctx, (GRect) {.origin = { 0   , 0 }, 
                                        .size   = { 71 , 83}}, 0, 0);
}
static void fill_rect_2(GContext* ctx, int fill_hrs, int fill_mins)
{
//     APP_LOG(APP_LOG_LEVEL_INFO, "fill_rect_2");

  set_colour(ctx, fill_hrs, fill_mins);
  graphics_fill_rect	(	 	ctx, (GRect) {.origin = { 73  , 43 }, 
                                        .size   = { 72 , 42}}, 0, 0);
}
static void fill_rect_1a(GContext* ctx, int fill_hrs, int fill_mins)
{
//     APP_LOG(APP_LOG_LEVEL_INFO, "fill_rect_1a");

  set_colour(ctx, fill_hrs, fill_mins);
  graphics_fill_rect	(	 	ctx, (GRect) {.origin = { 73  , 0 }, 
                                        .size   = { 35 , 41}}, 0, 0); 
}
static void fill_rect_1b(GContext* ctx, int fill_hrs, int fill_mins)
{
//   APP_LOG(APP_LOG_LEVEL_INFO, "fill_rect_1b");

  set_colour(ctx, fill_hrs, fill_mins);
  graphics_fill_rect	(	 	ctx, (GRect) {.origin = { 109  , 0 }, 
                                        .size   = { 35 , 41}}, 0, 0); 
}  



// Standard functions + time handler

static void mainwindow_unload(Window *window) {
   // Destroy
   text_layer_destroy(s_time_layer);
   layer_destroy(s_graph_layer);
   // BT indicator
   gbitmap_destroy(s_bt_icon_bitmap);
   bitmap_layer_destroy(s_bt_icon_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
     update_time();
}

static void init() {
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
                    .load = mainwindow_load,
                    .unload = mainwindow_unload
  });
    
  window_stack_push(s_main_window, true);
  
  // Register for Bluetooth connection updates
  bluetooth_connection_service_subscribe(bluetooth_callback);

}

static void deinit() {
  window_destroy(s_main_window);
}

int main (void){
  init();
  app_event_loop();
  deinit();
}

static void calc_fill (int block, int* hrs, int *mins, int *fill_h, int *fill_m)
{  
  *fill_h = *fill_m = 0;
  if ((*hrs-block) >= 0)  { *fill_h=1;   *hrs = *hrs - block; }
  if ((*mins-block) >= 0) { *fill_m=1;   *mins= *mins -block; }
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  char indicator;
  
  if (tick_time->tm_hour > 12){
    indicator = '.';
  }  
  else{
    indicator = ' ';
  }
  
  // debug , testing, screenshots
//    tick_time->tm_min = 59;  
//    tick_time->tm_hour = 12;
//   APP_LOG(APP_LOG_LEVEL_WARNING, "time: tm_sec=%d  tm_min=%d   tm_hour=%d", tick_time->tm_sec, tick_time->tm_min, tick_time->tm_hour);


  // Create a long-lived buffer
  static char buffer[] = "00:00 ";
  static char dateString[] = "wed 99/99  ";


  // Use 12 hour format
  
  strftime(buffer, sizeof("00:00"), "%l:%M" , tick_time);

  strncat (buffer, &indicator, 1);
  
  // Display TextLayers
  text_layer_set_text(s_time_layer, buffer);

  // Date
  strftime(dateString, sizeof(dateString), "%a %d/%m ", tick_time);
  text_layer_set_text(s_date_layer, dateString);

  
  
  
  
  // Now do colours
  int hrs, mins;
  char buf[] = "00";
  strftime(buf, sizeof("00"), "%l", tick_time);
  hrs=atoi(buf);
  strftime(buf, sizeof("00"), "%M", tick_time);
//   APP_LOG(APP_LOG_LEVEL_INFO, "colour calc, mins=%s", buf);

  mins=1+ atoi(buf)/5;
  
  
  APP_LOG(APP_LOG_LEVEL_INFO, "colour calc, h=%d  m=%d", hrs,mins);
  
  calc_fill (5, &hrs, &mins, &fill_h[0], &fill_m[0]);
  calc_fill (3, &hrs, &mins, &fill_h[1], &fill_m[1]);
  calc_fill (2, &hrs, &mins, &fill_h[2], &fill_m[2]);
  calc_fill (1, &hrs, &mins, &fill_h[3], &fill_m[3]);
  calc_fill (1, &hrs, &mins, &fill_h[4], &fill_m[4]);
  
    
//   int i; for (i=0; i<5;i++){
//        APP_LOG(APP_LOG_LEVEL_INFO, "       fill_h[%d]=%d     fill_m[%d]=%d", i,fill_h[i],i,fill_m[i]);
//   }
  
}
