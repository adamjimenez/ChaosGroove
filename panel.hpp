// Panel (.Hpp)
// ------------

#ifndef __Panel_hpp__
#define __Panel_hpp__ 

#define NUM_INFO_LINES				2

#define NO_SPELL						 -2

#define CONTROL_NO_SPELL			0
#define CONTROL_SKIP_SPELL		1
#define CONTROL_END_MOVE			2
#define CONTROL_END_FIRE			3
#define CONTROL_DISMOUNT			4
#define CONTROL_END_TURN			5

struct panel_t
{
 int info_area_x, info_area_y, info_area_w, info_area_h;
 bool info_area_show_piece_stats;
 int spell_area_x, spell_area_y, spell_area_w, spell_area_h, last_spell_icon_over;
 int spell_icon_w, spell_icon_h, spell_icon_space_x, spell_icon_space_y, spell_icon_amount;
 int stats_layer;
 
 struct alpha spell_area_control_alpha;
 struct alpha info_area_alpha;
 struct piece_data stats_piece;

 char info_area_line[NUM_INFO_LINES][512];
 Rgba info_area_line_rgba[NUM_INFO_LINES];
};

extern struct panel_t panel;

void setup_panels(void);
void do_panels_logic(void);
void draw_panels(void);

void draw_header_text(void);
void draw_creature_stats(piece_data creature);
void draw_spell_info(void);
void draw_alternate_info_panel(void);
void draw_spell_icons(void);
void draw_control_icons(void);
void find_control_icon_mouse_over(void);

#endif