#ifndef COBALTBLUECACHE
#define COBALTBLUECACHE

#include "CobaltBlueCore.h"
#include <map>

class CachedImage {
public:
  CachedImage(LPCWSTR filename);
  ~CachedImage();
  unsigned char* GetImage() const;
  unsigned Width() const;
  unsigned Height() const;
  unsigned char* RawData();

private:
  unsigned char* m_imageData;
  unsigned m_width;
  unsigned m_height;
};

class CobaltCache {
public:
  ~CobaltCache();
  const CachedImage* GetImage(LPCWSTR filename);

private:
  std::map<LPCWSTR, CachedImage*> m_cacheMap;
};

#endif