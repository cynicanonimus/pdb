#ifndef GDEFINITIONS_H
#define GDEFINITIONS_H
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
//GDefinitions.h
//
enum SEARCH_LOCATION
{
    enNODE_NAMES        = 0x1,
    enNODE_DESCRIPTOR   = 0x2,
    enATT_NAMES         = 0x4,
    enATT_BODY          = 0x8
};
//
#endif // GDEFINITIONS_H
