/*
 * Copyright 2026, Fabio Tomat <f.t.public@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <Window.h>
#include <AICommands.h>

class BTextView;
class BTextControl;
class BButton;
class BScrollView;

class MainWindow : public BWindow {
public:
    MainWindow(const char* context = nullptr);
    virtual ~MainWindow();

    virtual void MessageReceived(BMessage* msg);
    virtual bool QuitRequested();

private:
    void _OnSend();
    void _AppendText(const char* text);

    BTextView*    fHistoryView;
    BScrollView*  fHistoryScroll;
    BTextControl* fInputControl;
    BButton*      fSendButton;
    BButton*      fAbortButton;

    AIEngine*     fEngine;
};

#endif // MAIN_WINDOW_H
