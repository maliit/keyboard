// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; c-file-offsets: ((innamespace . 0)); -*-
/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Mohammad Anwari <Mohammad.Anwari@nokia.com>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "logic/layoutupdater.h"

#include <cstdlib>
#include <ctime>
#include <QCoreApplication>

int main(int argc,
         char ** argv)
{
    QCoreApplication app(argc, argv);
    double deadline(0);

    if (argc > 1) {
        deadline = std::atof(argv[1]);
        if (deadline < 0) {
            deadline = 0;
        }
    }

    int mode(0);

    if (argc > 2) {
        mode = 1;
    }

    MaliitKeyboard::Logic::LayoutHelper layout;
    MaliitKeyboard::Logic::LayoutUpdater updater;
    const QStringList ids (updater.keyboardIds());
    const int count(ids.size());

    // no sense in benchmarking one language - id won't change and no keyboard
    // loading will happen
    if (count < 2) {
        qDebug("No language files found.");
        return 1;
    }
    updater.setLayout(&layout);

    double total_time(0);
    double meeting_total_time(0);
    double missing_total_time(0);
    const int rounds(1000);
    int meeting_rounds(0);
    int missing_rounds(0);
    int previous_index(-1);
    QTime overall_mode_timer;
    int overall_counter(0);

    std::srand(time(0));
    if (mode) {
        overall_mode_timer.start();
        while (overall_mode_timer.elapsed() < deadline * 1000) {
            int index(std::rand() % count);

            // we want to be sure that we check different ids everytime.
            if (index == previous_index) {
                if (index + 1 == count) {
                    --index;
                } else {
                    ++index;
                }
            }
            previous_index = index;
            updater.setActiveKeyboardId(ids[index]);
            ++overall_counter;
        }
        qDebug("Done %d iterations in %f seconds.", overall_counter, deadline);
    } else {
        for (int iter(0); iter < rounds; ++iter) {
            int index(std::rand() % count);

            // we want to be sure that we check different ids everytime.
            if (index == previous_index) {
                if (index + 1 == count) {
                    --index;
                } else {
                    ++index;
                }
            }
            previous_index = index;

            QTime timer;

            timer.start();
            updater.setActiveKeyboardId(ids[index]);

            int this_time = timer.elapsed();

            total_time += this_time;
            if (deadline > 0) {
                if (this_time > deadline) {
                    ++missing_rounds;
                    missing_total_time += this_time;
                } else {
                    ++meeting_rounds;
                    meeting_total_time += this_time;
                }
            }

            if (deadline > 0) {
                qDebug("Deadline: %f", deadline);
                if (meeting_rounds > 0) {
                    qDebug("Iterations meeting deadline: %d, average %f ms, total time %f ms", meeting_rounds, meeting_total_time / meeting_rounds, meeting_total_time);
                }
                if (missing_rounds > 0) {
                    qDebug("Iterations missing deadline: %d, average %f ms. total time %f ms", missing_rounds, missing_total_time / missing_rounds, missing_total_time);
                }
            }
            qDebug("Iterations total: %d, average: %f ms, total time %f ms", rounds, total_time / rounds, total_time);
        }
    }
}
