//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

#include "BsCorePrerequisites.h"

namespace BansheeEngine
{
	/**
	 * @brief	Video mode contains information about how a render window
	 *			presents its information to an output device like a monitor.
	 */
	class BS_CORE_EXPORT VideoMode
	{
	public:
		VideoMode() {}

		/**
		 * @brief	Creates a new video mode.
		 *
		 * @param	width		Width of the frame buffer in pixels.
		 * @param	height		Height of the frame buffer in pixels.
		 * @param	refreshRate	How often should the output device refresh the output image in Hertz.
		 * @param	outputIdx	Output index of the output device. Normally this means
		 *						output monitor. 0th index always represents the primary device
		 *						while order of others is undefined.
		 */
		VideoMode(UINT32 width, UINT32 height, float refreshRate = 60.0f, UINT32 outputIdx = 0);
		virtual ~VideoMode();

		bool operator== (const VideoMode& other) const;

		/**
		 * @brief	Width of the front/back buffer in pixels.
		 */
		UINT32 getWidth() const { return mWidth; }

		/**
		 * @brief	Height of the front/back buffer in pixels.
		 */
		UINT32 getHeight() const { return mHeight; }

		/**
		 * @brief	Returns a refresh rate in Hertz.
		 */
		virtual float getRefreshRate() const { return mRefreshRate; }

		/**
		 * @brief	Returns information about the parent output.
		 */
		UINT32 getOutputIdx() const { return mOutputIdx; }

		/**
		 * @brief	Determines was video mode user created or provided by the API/OS.
		 *			API/OS created video modes can contain additional information that allows
		 *			the video mode to be used more accurately and you should use them when possible.
		 */
		bool isCustom() const { return mIsCustom;  }
	protected:
		UINT32 mWidth = 1280;
		UINT32 mHeight = 720;
		float mRefreshRate = 60.0f;
		UINT32 mOutputIdx = 0;
		bool mIsCustom = true;
	};

	/**
	 * @brief	Contains information about a video output device, including
	 *			a list of all available video modes.
	 */
	class BS_CORE_EXPORT VideoOutputInfo
	{
	public:
		VideoOutputInfo() { }
		virtual ~VideoOutputInfo();

		VideoOutputInfo(const VideoOutputInfo&) = delete; // Make non-copyable
		VideoOutputInfo& operator=(const VideoOutputInfo&) = delete; // Make non-copyable

		/**
		 * @brief	Name of the output device.
		 */
		const String& getName() const { return mName; }

		/**
		 * @brief	Number of available video modes for this output.
		 */
		UINT32 getNumVideoModes() const { return (UINT32)mVideoModes.size(); }

		/**
		 * @brief	Returns video mode at the specified index.
		 */
		const VideoMode& getVideoMode(UINT32 idx) const { return *mVideoModes.at(idx); }

		/**
		 * @brief	Returns the video mode currently used by the desktop.
		 */
		const VideoMode& getDesktopVideoMode() const { return *mDesktopVideoMode; }

	protected:
		String mName;
		Vector<VideoMode*> mVideoModes;
		VideoMode* mDesktopVideoMode = nullptr;
	};

	/**
	 * @brief	Contains information about available output devices (e.g. monitor) 
	 *			and their video modes.
	 */
	class BS_CORE_EXPORT VideoModeInfo
	{
	public:
		VideoModeInfo() { }
		virtual ~VideoModeInfo();

		VideoModeInfo(const VideoModeInfo&) = delete; // Make non-copyable
		VideoModeInfo& operator=(const VideoModeInfo&) = delete; // Make non-copyable

		/**
		 * @brief	Returns the number of available output devices.
		 */
		UINT32 getNumOutputs() const { return (UINT32)mOutputs.size(); }

		/**
		 * @brief	Returns video mode information about a specific output device.
		 */
		const VideoOutputInfo& getOutputInfo(UINT32 idx) const { return *mOutputs[idx]; }

	protected:
		Vector<VideoOutputInfo*> mOutputs;
	};
}