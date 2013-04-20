#ifndef WAITER_H
#define WAITER_H
//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
//
// waiter.h
//
#include <QObject>

class Waiter : public QObject
{
    Q_OBJECT
public:
    explicit Waiter(QObject *parent = 0);
    
    static void wait (int i_sec);

signals:
    
public slots:
    
};

#endif // WAITER_H
