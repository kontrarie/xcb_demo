#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

/* demo of using the XCB library
** to create coloured objects
** and coloured text
*/

int main() {

//  s e c t i o n   d e c l a r e   v a r i a b l e s

xcb_connection_t        *xcb_conn;
xcb_screen_t            *xcb_screen;
xcb_drawable_t          win;
xcb_colormap_t          colormap;
xcb_alloc_color_reply_t *creply;
xcb_gcontext_t          gc;
xcb_generic_event_t     *event;

uint32_t                mask;
uint32_t                values[9]; /* ruim groot genoeg */
uint32_t		xcb_font24;
char			*fontname_24="lucidasans-24";

// geometric objects
xcb_point_t          polyline[] = { {50, 10},
    { 5, 20},     /* rest of points are relative */
    {25,-20},
    {10, 10}};
xcb_segment_t        segments[] = { {100, 10, 140, 30}, {110, 25, 130, 60}};
xcb_rectangle_t      rectangles[] = { { 10, 50, 40, 20}, { 80, 50, 10, 40}};
xcb_arc_t            arcs[] = { {10, 100, 60, 40, 0, 90 << 6}, {90, 100, 55, 40, 0, 270 << 6}};

char	*demo_text = "Color demo XCB" ;
char    *title = "Color Demo" ;
int     xsize,ysize ;

//   s e c t i o n   "f u n c t i o n   d e f i n i t i o n s"

//   s e c t i o n   "t h e   r e a l   c o d e"

/* Get the first xcblib_screen */
xcb_conn = xcb_connect( 0, 0 ); if( !xcb_conn ) { printf(":: xcb_connect() failed\n"); return -1; }
win = xcb_generate_id(xcb_conn);
gc = xcb_generate_id(xcb_conn);
xcb_font24 = xcb_generate_id(xcb_conn);

xcb_screen = xcb_setup_roots_iterator(xcb_get_setup (xcb_conn)).data;
printf("Total screen dimensions = %d wide, %d high\n",xcb_screen->width_in_pixels,xcb_screen->height_in_pixels);

// create a window
mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
values[0] = xcb_screen->black_pixel;
values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS ;
xcb_create_window (xcb_conn,                      /* Connection      */
                   XCB_COPY_FROM_PARENT,          /* depth           */
                   win,                           /* window Id       */
                   xcb_screen->root,              /* parent window   */
                   0, 0,                          /* x, y            */
                   400,500,                       /* width, height   */
                   10,                            /* border_width    */
                   XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class           */
                   xcb_screen->root_visual,       /* visual          */
                   mask, values);                 /* masks           */
xcb_map_window(xcb_conn,win);

// give the window a title
xcb_change_property (xcb_conn, XCB_PROP_MODE_REPLACE, win,
 XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen (title), title);

// set up colors
uint32_t gList,gMask,pixel_green,pixel_blue,pixel_red,pixel_yellow,pixel_magenta,pixel_gray;
colormap=xcb_generate_id(xcb_conn);
xcb_create_colormap(xcb_conn, XCB_COLORMAP_ALLOC_NONE, colormap, win, xcb_screen->root_visual );
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 65535, 0, 0       ),0) ; pixel_red    = creply->pixel;
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 32767,32767,32767 ),0) ; pixel_gray   = creply->pixel;
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 0, 65535, 0       ),0) ; pixel_green  = creply->pixel;
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 0, 0, 65535       ),0) ; pixel_blue   = creply->pixel;
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 65535,65535,0     ),0) ; pixel_yellow = creply->pixel;
creply = xcb_alloc_color_reply(xcb_conn,xcb_alloc_color(xcb_conn,colormap, 65535,0,65535     ),0) ; pixel_magenta= creply->pixel;

// open up the named font
xcb_open_font(xcb_conn,xcb_font24,strlen(fontname_24),fontname_24);

// Create graphic context, & give it the font
mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT ;
//mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES | XCB_GC_FONT ;
values[0] = xcb_screen->white_pixel;
values[1] = xcb_screen->black_pixel;
//values[1] = 0;
values[2] = xcb_font24;
xcb_create_gc (xcb_conn, gc, win, mask, values);
xcb_close_font(xcb_conn,xcb_font24);

xcb_flush (xcb_conn);

// the real "program"
printf("Demo of using the XCB library to create coloured objects and coloured text\n");
printf("Press esc key to quit\n");

while ((event = xcb_wait_for_event (xcb_conn))) {
 switch (event->response_type & ~0x80) {
  case XCB_EXPOSE: {
   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_red; values[1]=xcb_screen->black_pixel;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_poly_line (xcb_conn, XCB_COORD_MODE_PREVIOUS, win, gc, 4, polyline);

   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_yellow; values[1]=xcb_screen->black_pixel;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_poly_segment (xcb_conn, win, gc, 2, segments);

   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_magenta; values[1]=xcb_screen->black_pixel;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_poly_rectangle (xcb_conn, win, gc, 2, rectangles);

   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_green; values[1]=xcb_screen->black_pixel;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_poly_arc (xcb_conn, win, gc, 2, arcs);

// now draw a big filled rectangle and write some text in it
   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_magenta; values[1]=xcb_screen->black_pixel;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_rectangle_t rectangle2[] = {{ 10, 250, 300, 90}};
   xcb_poly_fill_rectangle (xcb_conn,win,gc,1,rectangle2);

   // modify the GC to set the foreground color
   mask=XCB_GC_FOREGROUND | XCB_GC_BACKGROUND ; values[0]=pixel_green; values[1]=pixel_magenta;
   xcb_change_gc(xcb_conn,gc,mask,values);
   xcb_image_text_8 (xcb_conn,strlen(demo_text),win,gc,17,290,demo_text);

   xcb_flush (xcb_conn);
   break;
   }

  case XCB_KEY_RELEASE: {
   xcb_key_release_event_t *ev;
   ev = (xcb_key_release_event_t *)event;
   //printf("key pressed: %d\n",ev->detail);
   switch (ev->detail) {
   // ESC scan code, NOT ascii
   case 9:
    free (event);
    xcb_disconnect (xcb_conn);
    return 0;
    }
   }

   default: {
      // Unknown event type, ignore
      break;
   }
  }
  free (event);
 }
  return 0;
}
