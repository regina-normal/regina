
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#ifndef _REGINAVIEW_H_
#define _REGINAVIEW_H_

#include <qwidget.h>
#include <kparts/part.h>
#include <reginaiface.h>

class QPainter;
class KURL;

/**
 * This is the main view class for Regina.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * This regina uses an HTML component as an example.
 *
 * @short Main view
 * @author Ben Burton <bab@debian.org>
 * @version 4.0
 */
class ReginaView : public QWidget, virtual public ReginaIface
{
    Q_OBJECT
public:
	/**
	 * Default constructor
	 */
    ReginaView(QWidget *parent);

	/**
	 * Destructor
	 */
    virtual ~ReginaView();

    /**
     * Random 'get' function
     */
    QString currentURL();

    /**
     * Random 'set' function accessed by DCOP
     */
    virtual void openURL(QString url);

    /**
     * Random 'set' function
     */
    virtual void openURL(const KURL& url);

    /**
     * Print this view to any medium -- paper or not
     */
    void print(QPainter *, int height, int width);

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void slotOnURL(const QString& url);
    void slotSetTitle(const QString& title);

private:
    KParts::ReadOnlyPart *m_html;
};

#endif // _REGINAVIEW_H_
