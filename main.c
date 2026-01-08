#include "logic.h"
#include <raylib.h>
#include <stdlib.h>

NPC npcs[NUM_OF_NPCS];
Projectile fireballs[NUM_OF_NPCS];

int main(void) {
  srand(time(NULL));
  Direction direction = {0, 0};
  MovableObject player = create_player("Stefan");
  create_npcs(npcs);
  InitWindow(WIDTH, HEIGHT, "My Bouncy Text");
  SetTargetFPS(FPS_LIMIT);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    draw_character_movable(&player);
    spawn_npcs(npcs);
    attack_circle(&player, npcs);
    button_movement(&direction);
    move_player(&player, &direction);
    move_npcs(npcs, &player);
    // if (points_left <= 0 || player.hp <= 0) {
    //   EndDrawing();
    //   CloseWindow();
    //   return 0;
    // }
    ClosestTarget target = get_closest_target(npcs, &player);
    shoot_projectile(&player, fireballs, target);
    move_projectile(fireballs);
    draw_projectile(fireballs);
    current_status(&player);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
