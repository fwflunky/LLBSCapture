package main

import (
	"gocv.io/x/gocv"
	"sync"
	"syscall"
	"unsafe"
)

const (
	ScreenHeight = 720
	ScreenWidth  = 1280
)

type SharedVideoBuffer struct {
	FrameID int32
	Width   int32
	Height  int32
	Pixels  [ScreenWidth * ScreenHeight * 3]byte // BGR24
}

var (
	handle      syscall.Handle
	addr        unsafe.Pointer
	shared      *SharedVideoBuffer = nil
	lastFrameID int32              = -1
	mux         sync.Mutex
)

/* win package not included */
/* for pipe use the return value of streamer::createBuffer() in library.cpp */

func InitializeScreenCapture(pipe string) {
	var err error
	memNameUTF16, _ := syscall.UTF16PtrFromString(pipe)
	handle, err = win.CreateFileMapping(syscall.InvalidHandle, nil, syscall.PAGE_READONLY, 0, uint32(unsafe.Sizeof(SharedVideoBuffer{})), memNameUTF16)
	if err != nil {
		panic(err)
	}

	addr, err = win.MapViewOfFile(handle, syscall.FILE_MAP_READ, 0, 0, unsafe.Sizeof(SharedVideoBuffer{}))
	if err != nil || addr == nil {
		panic(err)
	}

	shared = (*SharedVideoBuffer)(addr)
}

func DestroyScreenCapture() {
	win.CloseHandle(handle)
	win.UnmapViewOfFile(addr)
}

func GetScreenMats() (bool, *gocv.Mat, *gocv.Mat) {
	mux.Lock()
	defer mux.Unlock()

	if shared.FrameID != lastFrameID {
		lastFrameID = shared.FrameID

		colorSrc, _ := gocv.NewMatFromBytes(ScreenHeight, ScreenWidth, gocv.MatTypeCV8UC3, shared.Pixels[:])
		scr := gocv.NewMat()
		gocv.CvtColor(colorSrc, &scr, gocv.ColorBGRToGray)
		return true, &colorSrc, &scr
	}

	return false, nil, nil
}
