#include "ui.hpp"

ui::Ui::Ui()
{
    EM_ASM_({
        var nav_graphics = document.getElementById('navGraphics');
        nav_graphics.addEventListener('change', function() {
            console.log("Hello from inline JavaScript!");
            // Check if the change event occurred on a radio button
            if (event.target.type === 'radio' && event.target.name === 'color') {
                // Get the selected value
                var selected_setting = event.target.value;

                // Call to Emscripten
                // console.log("Hello from inline JavaScript!");
                // Module.ccall('setGraphicsSetting', 'void', ['int'], [selected_setting]);
            }
        });
    });
}
