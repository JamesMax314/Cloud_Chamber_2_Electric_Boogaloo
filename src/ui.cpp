#include "ui.hpp"

ui::Ui::Ui()
{ 
}

void ui::Ui::poll_all()
{
    poll_graphics_setting();
    poll_free_move();
    poll_play_pause();
}

void ui::Ui::set_graphics_setting(int setting)
{
    graphics_setting = setting;
}

int ui::Ui::get_graphics_setting()
{
    return graphics_setting;
}

int ui::Ui::get_free_move_setting()
{
    return free_move_setting;
}

int ui::Ui::get_play_pause_setting()
{
    return pause_play_setting;
}

void ui::Ui::poll_graphics_setting()
{
    graphics_setting = EM_ASM_INT({
        var selected_setting = document.querySelector('input[name="graphics"]:checked').value;
        return selected_setting;
    });
}

void ui::Ui::poll_free_move()
{
    free_move_setting = EM_ASM_INT({
        var button_free_move = document.getElementById('button_free_move');
        var selected_setting = button_free_move.value;
        return selected_setting;
    });
}

void ui::Ui::poll_play_pause()
{
    pause_play_setting = EM_ASM_INT({
        var selected_setting = document.querySelector('input[name="pause_play"]:checked').value;
        return selected_setting;
    });
}
