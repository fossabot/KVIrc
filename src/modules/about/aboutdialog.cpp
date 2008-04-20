//=============================================================================
//
//   File : aboutdialog.cpp
//   Creation date : Sun Jun 23 17:59:12 2002 GMT by Szymon Stefanek
//
//   This file is part of the KVirc irc client distribution
//   Copyright (C) 2001 Szymon Stefanek (pragma at kvirc dot net)
//   Copyright (C) 2008 Elvio Basello (hellvis69 at netsons dot org)
//
//   This program is FREE software. You can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your opinion) any later version.
//
//   This program is distributed in the HOPE that it will be USEFUL,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program. If not, write to the Free Software Foundation,
//   Inc. ,59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
//=============================================================================

#include "aboutdialog.h"

#include "kvi_defaults.h"
#include "kvi_locale.h"
#include "kvi_app.h"
#include "kvi_fileutils.h"
#include "kvi_sourcesdate.h"
#include "kvi_buildinfo.h"
#include "kvi_osinfo.h"
#include <kvi_tal_textedit.h>

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QEvent>
#include <QCloseEvent>

extern KviAboutDialog * g_pAboutDialog;
/*
"<font color=\"#FFFF00\"><b>KVIrc public releases :</b></font><br>\n" \
"<br>\n" \
"<font color=\"#FF0000\">0.9.0</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: 25.01.1999</font><br>\n" \
"<br>\n" \
"<font color=\"#FF0000\">1.0.0 'Millennium'</font><br>\n" \
"<font size=\"2\">\"The net in your hands\"</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: 21.12.1999</font><br>\n" \
"<br>\n" \
"<font color=\"#FF0000\">2.0.0 'Phoenix'</font><br>\n" \
"<font size=\"2\">\"The client that can't make coffee\"</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: 30.05.2000</font><br>\n" \
"<br>\n" \
"<font color=\"#FF0000\">2.1.0 'Dark Star'</font><br>\n" \
"<font size=\"2\">\"The client that can't make coffee\"</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: 30.01.2001</font><br>\n" \
"<br>\n" \
"<font color=\"#FF0000\">2.1.1 'Monolith'</font><br>\n" \
"<font size=\"2\">\"A breath of fresh net\"</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: 01.05.2001</font><br>\n" \
"<br> 3.0.0-xmas build: 24-12-2001\n" \
"3.0.0-beta1: 24-06-2002\n "
"<font color=\"#FF0000\">3.0.0 'Avatar'</font><br>\n" \
"<font size=\"2\">\"No slogan yet\"</font><br>\n" \
"<font size=\"2\" color=\"#808080\">Release date: Still unknown</font><br>\n" \
*/

#include "abouttext.inc"

KviAboutDialog::KviAboutDialog()
: KviTalTabDialog(0)
{
	setWindowTitle(__tr2qs_ctx("About KVIrc...","about"));
	setOkButton(__tr2qs_ctx("Close","about"));

	// About tab
	KviStr buffer;
	g_pApp->findImage(buffer,"kvi_splash.png");

	QPixmap pix(buffer.ptr());

	QWidget * w = new QWidget(this);
	QGridLayout * g = new QGridLayout(w,2,1,4,8);

	QLabel * l = new QLabel(w);
	l->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	l->setBackgroundColor(Qt::black);
	l->setAlignment(Qt::AlignCenter);
	l->setPixmap(pix);

	g->addWidget(l,0,0);

	QString aboutString= "KVIrc <b>" KVI_VERSION " '" KVI_RELEASE_NAME "'</b><br>";
	aboutString += __tr2qs_ctx("Forged by the <b>KVIrc Development Team</b>","about");

	l = new QLabel(aboutString,w);
	l->setAlignment(Qt::AlignCenter);
	g->addWidget(l,1,0);

	addTab(w,__tr2qs_ctx("About","about"));


	// Info tab
	w = new QWidget(this);
	g = new QGridLayout(w,1,1,4,8);

	KviTalTextEdit * v = new KviTalTextEdit(w);
	v->setReadOnly(true);
	g->addWidget(v,0,0);

	// Get info
	QString infoString = "<b>KVIrc " KVI_VERSION " '" KVI_RELEASE_NAME "'</b><br><br>";
	infoString += "<b>";
	infoString += __tr2qs_ctx("Runtime Info","about");
	infoString += ":</b><br>";
	infoString += __tr2qs_ctx("System Name","about");
	infoString += ": ";
	infoString += KviOsInfo::name();
	infoString += " ";
	infoString += KviOsInfo::release();
	infoString += "<br>";
	infoString += __tr2qs_ctx("System Version","about");
	infoString += ": ";
	infoString += KviOsInfo::version();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Architecture","about");
	infoString += ": ";
	infoString += KviOsInfo::machine();
	infoString += "<br><br>";
	infoString += "<b>";
	infoString += __tr2qs_ctx("Build Info","about");
	infoString += ":</b><br>";
// FIXME: Remove this line when win32 compiles using CMake
#ifndef COMPILE_ON_WINDOWS
	infoString += __tr2qs_ctx("Build Date","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildDate();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Sources Date","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildSourcesDate();
	infoString += "<br>";
	infoString += __tr2qs_ctx("System Name","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildSystem();
	infoString += "<br>";
	infoString += __tr2qs_ctx("CPU Name","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildCPU();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Build Command","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildCommand();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Build Flags","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildFlags();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Compiler Name","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildCompiler();
	infoString += "<br>";
	infoString += __tr2qs_ctx("Compiler Flags","about");
	infoString += ": ";
	infoString += KviBuildInfo::buildCompilerFlags();
#endif
	v->setText(infoString);

	addTab(w,__tr2qs_ctx("Executable Informations","about"));


	// Honor & Glory tab
	w = new QWidget(this);
	g = new QGridLayout(w,1,1,4,8);

	v = new KviTalTextEdit(w);
	v->setReadOnly(true);
	g->addWidget(v,0,0);

	v->setText(g_szAboutText);

	addTab(w,__tr2qs_ctx("Honor && Glory","about"));


	// License tab
	w = new QWidget(this);
	g = new QGridLayout(w,1,1,4,8);

	v = new KviTalTextEdit(w);
	v->setReadOnly(true);
	v->setWordWrap(KviTalTextEdit::NoWrap);
	g->addWidget(v,0,0);

	QString szLicense;

	QString szLicensePath;
	g_pApp->getGlobalKvircDirectory(szLicensePath,KviApp::License,"COPYING");
	
	if(!KviFileUtils::loadFile(szLicensePath,szLicense))
	{
		szLicense = __tr2qs_ctx("Oops... Can't find the license file...\n" \
			"It MUST be included in the distribution...\n" \
			"Please report to <pragma at kvirc dot net>","about");
	}

	v->setText(szLicense);

	addTab(w,__tr2qs_ctx("License","about"));


	connect(this,SIGNAL(applyButtonPressed()),this,SLOT(closeButtonPressed()));
}

KviAboutDialog::~KviAboutDialog()
{
	g_pAboutDialog = 0;
}

void KviAboutDialog::closeEvent(QCloseEvent *e)
{
	e->ignore();
	delete this;
}

void KviAboutDialog::closeButtonPressed()
{
	delete this;
}

#ifndef COMPILE_USE_STANDALONE_MOC_SOURCES
#include "aboutdialog.moc"
#endif //!COMPILE_USE_STANDALONE_MOC_SOURCES

