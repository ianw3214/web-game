"use strict";

let moveSystem = {
    generate: function(speed = 100) {
        return {
            up: false,
            down: false,
            left: false,
            right: false,
            was_moving: false,
            // The data of the move component
            speed: speed
        }
    },
    update: function (entities, delta) {
        for (let i in entities) {
            let entity = entities[i];
            let moving = false;
            if (entity.hasOwnProperty("position") && entity.hasOwnProperty("move")) {
                if (entity.move.hasOwnProperty("speed")) {
                    let distance = entity.move.speed * delta / 1000;
                    if (entity.hasOwnProperty("scaling")) {
                        if (entity.scaling.scaling === true) {
                            distance *= scalingSystem.scale;
                        }
                    }
                    if (entity.move.up) {
                        moving = true;
                        entity.position.y -= distance;
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_UP";
                        }
                    }
                    if (entity.move.down) {
                        moving = true;
                        entity.position.y += distance;
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_DOWN";
                        }
                    }
                    if (entity.move.left) {
                        moving = true;
                        entity.position.x -= distance;
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_LEFT";
                        }
                    }
                    if (entity.move.right) {
                        moving = true;
                        entity.position.x += distance;
                        if (entity.hasOwnProperty("animation")) {
                            entity.animation.hint = "RUN_RIGHT";
                        }
                    }
                }
            }
            if (entity.move.was_moving && moving == false) {
                if (entity.hasOwnProperty("animation")) {
                    entity.animation.hint = "IDLE";
                }
            }
            // RESET MOVEMENT AFTER HANDLING
            entity.move = {
                up: false,
                down: false,
                left: false,
                right: false,
                was_moving: moving,
                // Other data to carry along
                speed: entity.move.speed
            };
        }
    }
}