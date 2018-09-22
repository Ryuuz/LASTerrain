#ifndef LASDEFINITIONS_H
#define LASDEFINITIONS_H

struct PUBLIC_HEADER_BLOCK
{
    unsigned char versionMinor;
    unsigned long offsetToPointData;
    unsigned short pointDataRecordLength;
    unsigned long legacyNumberOfPointRecords;
    double xScaleFactor;
    double yScaleFactor;
    double zScaleFactor;
    double xOffset;
    double yOffset;
    double zOffset;
    unsigned long long numberOfPointRecords;
};

#endif // LASDEFINITIONS_H
