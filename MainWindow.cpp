/*
 * Copyright 2026, Fabio Tomat <f.t.public@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "MainWindow.h"
#include <TextView.h>
#include <TextControl.h>
#include <Button.h>
#include <ScrollView.h>
#include <LayoutBuilder.h>
#include <Application.h>
#include <String.h>
#include <stdio.h>

static const uint32 MSG_SEND_CLICKED = 'SEND';

MainWindow::MainWindow(const char* context)
    : BWindow(BRect(100, 100, 600, 500), "Haiku MCP Client", B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    // Inizializza l'Engine di Intelligenza Artificiale
    if (context != nullptr && context[0] != '\0') {
        fEngine = new AIEngine(context);
    } else {
        fEngine = new AIEngine();
    }

    // Semplice layout di chat
    fHistoryView = new BTextView("history");
    fHistoryView->MakeEditable(false);
    
    if (context != nullptr && context[0] != '\0') {
        BString title;
        if (fEngine->GetTitle(title) == B_OK && !title.IsEmpty()) {
            SetTitle(title.String());
            BString msg;
            msg.SetToFormat("[Contesto: %s]\n\n", title.String());
            _AppendText(msg.String());
        } else {
            BString msg;
            msg.SetToFormat("[Ripristinato contesto: %s]\n\n", context);
            _AppendText(msg.String());
        }
    }
    fHistoryView->MakeEditable(false);
    
    fHistoryScroll = new BScrollView("history_scroll", fHistoryView, B_WILL_DRAW, false, true);

    fInputControl = new BTextControl("input", "", "", new BMessage(MSG_SEND_CLICKED));
    fSendButton = new BButton("send", "Invia", new BMessage(MSG_SEND_CLICKED));

    // Imposta la dimensione dei pulsanti per adattarsi perfettamente
    fSendButton->SetExplicitMaxSize(BSize(100, B_SIZE_UNSET));

    // Costruisci il layout
    BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
        .SetInsets(10)
        .Add(fHistoryScroll, 10) // Pesa molto per occupare il massimo dello schermo
        .AddGroup(B_HORIZONTAL, 5, 1)
            .Add(fInputControl, 8)
            .Add(fSendButton, 2)
        .End()
    .End();

    // Sposta il focus sul box di testo per poter digitare subito
    fInputControl->MakeFocus(true);
}

MainWindow::~MainWindow()
{
    //delete fEngine;
}

void MainWindow::MessageReceived(BMessage* msg)
{
    switch (msg->what) {
        case MSG_SEND_CLICKED:
            _OnSend();
            break;
        case MSG_AI_RESPONSE: {
            bool complete = false;
            msg->FindBool("complete", &complete);
            
            if (!complete) {
                // Pezzettino di stream asincrono arrivato
                BString partialToken = msg->FindString("partial");
                if (partialToken.Length() > 0) {
                    _AppendText(partialToken.String());
                    // Scorri in automatico verso il basso per seguire lo stream
                    fHistoryView->ScrollToSelection();
                }
            } else {
                // Generazione completata con successo
                BString response = msg->FindString("response");
                status_t status = B_OK;
                msg->FindInt32("status", &status);
                
                if (status != B_OK) {
                    _AppendText("\n[Errore di generazione]\n");
                } else {
                    _AppendText("\n\n");
                }
                
                fHistoryView->ScrollToSelection();
                fInputControl->SetEnabled(true);
                fSendButton->SetEnabled(true);
                fInputControl->MakeFocus(true);
            }
            break;
        }
        case MSG_AI_ERROR: {
            _AppendText("\n[Errore del Server o del Plugin]\n");
            fHistoryView->ScrollToSelection();
            fInputControl->SetEnabled(true);
            fSendButton->SetEnabled(true);
            fInputControl->MakeFocus(true);
            break;
        }
        default:
            BWindow::MessageReceived(msg);
            break;
    }
}

bool MainWindow::QuitRequested()
{
    be_app->PostMessage(B_QUIT_REQUESTED);
    return true;
}

void MainWindow::_OnSend()
{
    BString text = fInputControl->Text();
    text.Trim();
    if (text.IsEmpty()) return;

    // Disabilita i controlli durante l'interazione per prevenire spam
    fInputControl->SetEnabled(false);
    fSendButton->SetEnabled(false);

    // Mostra il prompt dell'utente nello storico della conversazione
    _AppendText("Tu: ");
    _AppendText(text.String());
    _AppendText("\n\nGemini: ");
    fHistoryView->ScrollToSelection();

    // Svuota la barra di inserimento
    fInputControl->SetText("");

    // Invia la richiesta asincrona all'ai_server passandogli questo BMessenger per lo stream
    status_t err = fEngine->GenerateAsync(text.String(), BMessenger(this));
    if (err != B_OK) {
        _AppendText("[Errore di connessione al server]\n\n");
        fHistoryView->ScrollToSelection();
        fInputControl->SetEnabled(true);
        fSendButton->SetEnabled(true);
        fInputControl->MakeFocus(true);
    }
}

void MainWindow::_AppendText(const char* text)
{
    if (text == nullptr || text[0] == '\0')
        return;
    
    int32 len = fHistoryView->TextLength();
    fHistoryView->Select(len, len);
    fHistoryView->Insert(text);
}
