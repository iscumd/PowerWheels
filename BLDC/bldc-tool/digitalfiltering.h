/*
    Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef DIGITALFILTERING_H
#define DIGITALFILTERING_H

#include <QVector>

class DigitalFiltering
{
public:
    DigitalFiltering();
    void fft(int dir,int m,double *real,double *imag);
    void dft(int dir, int len, double *real, double *imag);
    void fftshift(double *data, int len);
    void hamming(double *data, int len);
    void zeroPad(double *data, double *result, int dataLen, int resultLen);
    int whichPowerOfTwo(unsigned int number);

    QVector<double> filterSignal(const QVector<double> &signal, const QVector<double> &filter, bool padAfter = false);
    QVector<double> generateFirFilter(double f_break, int bits, bool useHamming);
    QVector<double> fftWithShift(QVector<double> &signal, int resultBits, bool scaleByLen = false);
};

#endif // DIGITALFILTERING_H
