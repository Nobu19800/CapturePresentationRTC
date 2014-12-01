#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import ctypes
 
def main():
    wx = ctypes.windll.user32.GetSystemMetrics(0)
    wy = ctypes.windll.user32.GetSystemMetrics(1)

    print u"スライドの幅を入力してください"
    sx = float(sys.stdin.readline())
    print u"スライドの高さを入力してください"
    sy = float(sys.stdin.readline())

    w = int(sx/sy*wy)

    ans = int((wx - w)/2)

    
    print u"左上の座標",ans,0
    print u"幅、高さ",w,wy

    sys.stdin.readline()
    
 
if __name__ == "__main__":
    main()
