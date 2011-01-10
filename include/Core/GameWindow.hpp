/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
