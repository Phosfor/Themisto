/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _GAME_WINDOW_HPP_
#define _GAME_WINDOW_HPP_

class GameWindow : public CL_Window
{
    public:
        GameWindow(CL_GUIManager *manager, const CL_DisplayWindowDescription &desc)
        : CL_Window(manager, desc)
        {
            func_render().set(this, &GameWindow::on_render);
            set_constant_repaint(true);
        }

    private:
        void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
        {
            update_game_logic();
            render_game(gc);
        }
};

#endif /* _GAME_WINDOW_HPP_ */
