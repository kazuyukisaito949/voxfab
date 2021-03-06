/*******************************************************************************
Copyright (c) 2016, Atsushi Masumori
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

// This software is developed based on VoxCad (https://svn.code.sf.net/p/voxcad/code/VoxCad/) developed by following author with following license.

/*******************************************************************************
Copyright (c) 2010, Jonathan Hiller
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * The name if its contributors may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "QThreadWrap.h"
#include <QProgressDialog>
#include <QApplication>
#include <time.h>
#include <QMessageBox>
#include <sstream>
#include <iostream>

Thread::Thread()
{
	ClearAll();
}

Thread::Thread(QString* SArg1In, bool WantTimedIn)
{
	ClearAll();
	SArg1 = SArg1In;
	WantTimed = WantTimedIn;
}

Thread::~Thread()
{

}

void Thread::run() //called automatically when we start the thread
{
	emit CallFunc(SArg1); //must be directly connected to thread for progress bar to work! (
}

void Thread::Execute(bool WaitTilDone) { //starts the thread, waits til complete;
	clock_t startTime, finishTime;
	startTime = clock();
	QProgressDialog progress;
	progress.setModal(true);
	if (pCancelFlag) *pCancelFlag = false; //haven't canceled before we started!

	start(); 

	if (WantProgress) progress.show();

    if (WantProgress || WantTimed || WaitTilDone){ //if we want to stick around until the thread is finished...
		while (isRunning()){
			if (WantProgress){
				progress.setLabelText(QString((*pCurMessage).c_str()));
				progress.setRange(0, *pCurMaxTick);
				progress.setValue(*pCurTick);
				QApplication::processEvents();
				if (progress.wasCanceled()){
					*pCancelFlag = true;
					wait();
					return; //returns
				}
			}
			msleep(50);
		}
	}

	finishTime = clock();
	double duration = (double)(finishTime - startTime) / CLOCKS_PER_SEC;
	std::ostringstream os;
	os << "Elapsed time: " << duration << " Sec";
	if (WantTimed) QMessageBox::warning(NULL, "Warning", (os.str()).c_str());
}


