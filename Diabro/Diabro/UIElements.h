#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "Ogre.h"
#include "OgreOverlaySystem.h"
#include <math.h>
#include "OgreTimer.h"
#include "InputContext.h"

#if OGRE_UNICODE_SUPPORT
#	if	OGRE_STRING_USE_CUSTOM_MEMORY_ALLOCATOR
#		define DISPLAY_STRING_TO_STRING(DS) (DS.asUTF8_c_str())
#	else
#		define DISPLAY_STRING_TO_STRING(DS) (DS.asUTF8())
#	endif
#else
#define DISPLAY_STRING_TO_STRING(DS) (DS)
#endif
namespace DiabroUI
{
	enum AnchorLocation   // enumerator values for widget tray anchoring locations
	{
		TOPLEFT,
		TOP,
		TOPRIGHT,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOMLEFT,
		BOTTOM,
		BOTTOMRIGHT,
		NONE
	};

	//TODO: implement buttonstates when menu is gonna be build

	class Widget;
	class HUDText;
	class DialogTextBox;
	class MiniMap;
	class ImageWidget;
	class Bar;

	//TODO: maybe baby we need a SdkTrayListener

	/*=============================================================================
	| Abstract base class for all widgets.
	=============================================================================*/
	class Widget
	{
	public:

		Widget()
		{
			mTrayLoc = NONE;
			mElement = 0;
		}

		virtual ~Widget() {}

		void cleanup()
		{
			if (mElement) nukeOverlayElement(mElement);
			mElement = 0;
		}

		/*-----------------------------------------------------------------------------
		| Static utility method to recursively delete an overlay element plus
		| all of its children from the system.
		-----------------------------------------------------------------------------*/
		static void nukeOverlayElement(Ogre::OverlayElement* element)
		{
			Ogre::OverlayContainer* container = dynamic_cast<Ogre::OverlayContainer*>(element);
			if (container)
			{
				std::vector<Ogre::OverlayElement*> toDelete;

				Ogre::OverlayContainer::ChildIterator children = container->getChildIterator();
				while (children.hasMoreElements())
				{
					toDelete.push_back(children.getNext());
				}

				for (unsigned int i = 0; i < toDelete.size(); i++)
				{
					nukeOverlayElement(toDelete[i]);
				}
			}
			if (element)
			{
				Ogre::OverlayContainer* parent = element->getParent();
				if (parent) parent->removeChild(element->getName());
				Ogre::OverlayManager::getSingleton().destroyOverlayElement(element);
			}
		}

		/*-----------------------------------------------------------------------------
		| Static utility method used to get the width of a caption in a text area.
		-----------------------------------------------------------------------------*/
		static Ogre::Real getCaptionWidth(const Ogre::DisplayString& caption, Ogre::TextAreaOverlayElement* area)
		{
			Ogre::Font* font = (Ogre::Font*)Ogre::FontManager::getSingleton().getByName(area->getFontName()).getPointer();
			Ogre::String current = DISPLAY_STRING_TO_STRING(caption);
			Ogre::Real lineWidth = 0;

			for (unsigned int i = 0; i < current.length(); i++)
			{
				// be sure to provide a line width in the text area
				if (current[i] == ' ')
				{
					if (area->getSpaceWidth() != 0) lineWidth += area->getSpaceWidth();
					else lineWidth += font->getGlyphAspectRatio(' ') * area->getCharHeight();
				}
				else if (current[i] == '\n') break;
				// use glyph information to calculate line width
				else lineWidth += font->getGlyphAspectRatio(current[i]) * area->getCharHeight();
			}

			return (unsigned int)lineWidth;
		}

		/*-----------------------------------------------------------------------------
		| Static utility method to cut off a string to fit in a text area.
		-----------------------------------------------------------------------------*/
		static void fitCaptionToArea(const Ogre::DisplayString& caption, Ogre::TextAreaOverlayElement* area, Ogre::Real maxWidth)
		{
			Ogre::Font* f = (Ogre::Font*)Ogre::FontManager::getSingleton().getByName(area->getFontName()).getPointer();
			Ogre::String s = DISPLAY_STRING_TO_STRING(caption);

			size_t nl = s.find('\n');
			if (nl != Ogre::String::npos) s = s.substr(0, nl);

			Ogre::Real width = 0;

			for (unsigned int i = 0; i < s.length(); i++)
			{
				if (s[i] == ' ' && area->getSpaceWidth() != 0) width += area->getSpaceWidth();
				else width += f->getGlyphAspectRatio(s[i]) * area->getCharHeight();
				if (width > maxWidth)
				{
					s = s.substr(0, i);
					break;
				}
			}

			area->setCaption(s);
		}

		Ogre::OverlayElement* getOverlayElement()
		{
			return mElement;
		}

		const Ogre::String& getName()
		{
			return mElement->getName();
		}

		AnchorLocation getTrayLocation()
		{
			return mTrayLoc;
		}

		void hide()
		{
			mElement->hide();
		}

		void show()
		{
			mElement->show();
		}

		bool isVisible()
		{
			return mElement->isVisible();
		}

		// callbacks

		virtual void _cursorPressed(const Ogre::Vector2& cursorPos) {}
		virtual void _cursorReleased(const Ogre::Vector2& cursorPos) {}
		virtual void _cursorMoved(const Ogre::Vector2& cursorPos) {}
		virtual void _focusLost() {}

		// internal methods used by UIElementsManager. do not call directly.

		void _assignToTray(AnchorLocation trayLoc) { mTrayLoc = trayLoc; }

	protected:

		Ogre::OverlayElement* mElement;
		AnchorLocation mTrayLoc;
	};

	typedef std::vector<Widget*> WidgetList;
	typedef Ogre::VectorIterator<WidgetList> WidgetIterator;

	/*=============================================================================
	| HUD text widget.
	=============================================================================*/
	class HUDText : public Widget
	{
	public:

		// Do not instantiate any widgets directly. Use UIElementsManager.
		HUDText(const Ogre::String& name, const Ogre::DisplayString& text, Ogre::Real width, Ogre::Real height)
		{
			mElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("UI/HUDText", "Panel", name);
			mElement->setWidth(width);
			mElement->setHeight(height);
			Ogre::OverlayContainer* container = (Ogre::OverlayContainer*)mElement;
			mTextArea = (Ogre::TextAreaOverlayElement*)container->getChild(getName() + "/HUDCaption");
			mPadding = 15;
			mText = text;
			refitContents();
		}

		void setPadding(Ogre::Real padding)
		{
			mPadding = padding;
			refitContents();
		}

		Ogre::Real getPadding()
		{
			return mPadding;
		}

		void setWidth(Ogre::Real width) {
			mElement->setWidth(width);
			//refitContents();
		}

		const Ogre::DisplayString& getText()
		{
			return mText;
		}

		/*-----------------------------------------------------------------------------
		| Sets text box content. Most of this method is for wordwrap.
		-----------------------------------------------------------------------------*/
		void setText(const Ogre::DisplayString& text)
		{
			mText = text;
			mLines.clear();

			Ogre::Font* font = (Ogre::Font*)Ogre::FontManager::getSingleton().getByName(mTextArea->getFontName()).getPointer();

			Ogre::String current = DISPLAY_STRING_TO_STRING(text);
			bool firstWord = true;
			unsigned int lastSpace = 0;
			unsigned int lineBegin = 0;
			Ogre::Real lineWidth = 0;
			//Ogre::Real rightBoundary = mElement->getWidth() - 2 * mPadding + mScrollTrack->getLeft() + 10;

			for (unsigned int i = 0; i < current.length(); i++)
			{
				if (current[i] == ' ')
				{
					if (mTextArea->getSpaceWidth() != 0) lineWidth += mTextArea->getSpaceWidth();
					else lineWidth += font->getGlyphAspectRatio(' ') * mTextArea->getCharHeight();
					firstWord = false;
					lastSpace = i;
				}
				else if (current[i] == '\n')
				{
					firstWord = true;
					lineWidth = 0;
					mLines.push_back(current.substr(lineBegin, i - lineBegin));
					lineBegin = i + 1;
				}
				else
				{
					// use glyph information to calculate line width
					lineWidth += font->getGlyphAspectRatio(current[i]) * mTextArea->getCharHeight();

				}
			}

			mLines.push_back(current.substr(lineBegin));

			mTextArea->setCaption(current);
		}

		/*-----------------------------------------------------------------------------
		| Sets text box content horizontal alignment.
		-----------------------------------------------------------------------------*/
		void setTextAlignment(Ogre::TextAreaOverlayElement::Alignment ta)
		{
			if (ta == Ogre::TextAreaOverlayElement::Left) mTextArea->setHorizontalAlignment(Ogre::GHA_LEFT);
			else if (ta == Ogre::TextAreaOverlayElement::Center) mTextArea->setHorizontalAlignment(Ogre::GHA_CENTER);
			else mTextArea->setHorizontalAlignment(Ogre::GHA_RIGHT);
			refitContents();
		}

		void clearText()
		{
			setText("");
		}

		void appendText(const Ogre::DisplayString& text)
		{
			setText(getText() + text);
		}

		/*-----------------------------------------------------------------------------
		| Makes adjustments based on new padding, size, or alignment info.
		-----------------------------------------------------------------------------*/
		void refitContents()
		{
			setText(getText());
		}

		/*-----------------------------------------------------------------------------
		| Gets how many lines of text can fit in this window.
		-----------------------------------------------------------------------------*/
		unsigned int getHeightInLines()
		{
			return (unsigned int)((mElement->getHeight() - 2 * mPadding /*- mCaptionBar->getHeight() + 5)*/) / mTextArea->getCharHeight());
		}

	protected:
		Ogre::TextAreaOverlayElement* mTextArea;
		Ogre::DisplayString mText;
		Ogre::StringVector mLines;
		Ogre::Real mPadding;
		unsigned int mStartingLine;
	};

	/*=============================================================================
	| Scrollable text box widget.
	=============================================================================*/
	class DialogTextBox : public Widget
	{
	public:

		// Do not instantiate any widgets directly. Use UIElementsManager.
		DialogTextBox(const Ogre::String& name, const Ogre::DisplayString& caption, Ogre::Real width, Ogre::Real height)
		{
			mElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("UI/DialogBox", "BorderPanel", name);
			mElement->setWidth(width);
			mElement->setHeight(height);
			Ogre::OverlayContainer* container = (Ogre::OverlayContainer*)mElement;
			mTextArea = (Ogre::TextAreaOverlayElement*)container->getChild(getName() + "/DialogText");

			Ogre::TextAreaOverlayElement* continueText = (Ogre::TextAreaOverlayElement*)container->getChild(getName() + "/ContinueText");
			continueText->setCaption("Press SPACE to continue");

			mCaptionBar = (Ogre::BorderPanelOverlayElement*)container->getChild(getName() + "/DialogCaptionBar");
			mCaptionBar->setWidth(width - 4);
			mCaptionTextArea = (Ogre::TextAreaOverlayElement*)mCaptionBar->getChild(mCaptionBar->getName() + "/DialogCaption");
			setCaption(caption);
			mPadding = 15;
			mText = "";
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
			mTextArea->setCharHeight(mTextArea->getCharHeight() - 3);
			mCaptionTextArea->setCharHeight(mCaptionTextArea->getCharHeight() - 3);
	#endif
			refitContents();
		}

		void setPadding(Ogre::Real padding)
		{
			mPadding = padding;
			refitContents();
		}

		Ogre::Real getPadding()
		{
			return mPadding;
		}

		const Ogre::DisplayString& getCaption()
		{
			return mCaptionTextArea->getCaption();
		}

		void setCaption(const Ogre::DisplayString& caption)
		{
			mCaptionTextArea->setCaption(caption);
		}

		const Ogre::DisplayString& getText()
		{
			return mText;
		}

		/*-----------------------------------------------------------------------------
		| Sets text box content. Most of this method is for wordwrap.
		-----------------------------------------------------------------------------*/
		void setText(const Ogre::DisplayString& text)
		{
			mText = text;
			mLines.clear();

			Ogre::Font* font = (Ogre::Font*)Ogre::FontManager::getSingleton().getByName(mTextArea->getFontName()).getPointer();

			Ogre::String current = DISPLAY_STRING_TO_STRING(text);
			bool firstWord = true;
			unsigned int lastSpace = 0;
			unsigned int lineBegin = 0;
			Ogre::Real lineWidth = 0;
			Ogre::Real rightBoundary = mElement->getWidth() - 2 * mPadding;// +mScrollTrack->getLeft() + 10;

			for (unsigned int i = 0; i < current.length(); i++)
			{
				if (current[i] == ' ')
				{
					if (mTextArea->getSpaceWidth() != 0) lineWidth += mTextArea->getSpaceWidth();
					else lineWidth += font->getGlyphAspectRatio(' ') * mTextArea->getCharHeight();
					firstWord = false;
					lastSpace = i;
				}
				else if (current[i] == '\n')
				{
					firstWord = true;
					lineWidth = 0;
					mLines.push_back(current.substr(lineBegin, i - lineBegin));
					lineBegin = i + 1;
				}
				else
				{
					// use glyph information to calculate line width
					lineWidth += font->getGlyphAspectRatio(current[i]) * mTextArea->getCharHeight();
					if (lineWidth > rightBoundary)
					{
						if (firstWord)
						{
							current.insert(i, "\n");
							i = i - 1;
						}
						else
						{
							current[lastSpace] = '\n';
							i = lastSpace - 1;
						}
					}
				}
			}

			mLines.push_back(current.substr(lineBegin));

			mTextArea->setCaption(current);
		}

		/*-----------------------------------------------------------------------------
		| Sets text box content horizontal alignment.
		-----------------------------------------------------------------------------*/
		void setTextAlignment(Ogre::TextAreaOverlayElement::Alignment ta)
		{
			if (ta == Ogre::TextAreaOverlayElement::Left) mTextArea->setHorizontalAlignment(Ogre::GHA_LEFT);
			else if (ta == Ogre::TextAreaOverlayElement::Center) mTextArea->setHorizontalAlignment(Ogre::GHA_CENTER);
			else mTextArea->setHorizontalAlignment(Ogre::GHA_RIGHT);
			refitContents();
		}

		void clearText()
		{
			setText("");
		}

		void appendText(const Ogre::DisplayString& text)
		{
			setText(getText() + text);
		}

		/*-----------------------------------------------------------------------------
		| Makes adjustments based on new padding, size, or alignment info.
		-----------------------------------------------------------------------------*/
		void refitContents()
		{
			mTextArea->setTop(mCaptionBar->getHeight() + mPadding - 5);
			if (mTextArea->getHorizontalAlignment() == Ogre::GHA_RIGHT) mTextArea->setLeft(-mPadding);// +mScrollTrack->getLeft());
			else if (mTextArea->getHorizontalAlignment() == Ogre::GHA_LEFT) mTextArea->setLeft(mPadding);

			setText(getText());
		}

		/*-----------------------------------------------------------------------------
		| Gets how many lines of text can fit in this window.
		-----------------------------------------------------------------------------*/
		unsigned int getHeightInLines()
		{
			return (unsigned int)((mElement->getHeight() - 2 * mPadding - mCaptionBar->getHeight() + 5) / mTextArea->getCharHeight());
		}

	protected:
		Ogre::TextAreaOverlayElement* mTextArea;
		Ogre::BorderPanelOverlayElement* mCaptionBar;
		Ogre::TextAreaOverlayElement* mCaptionTextArea;
		Ogre::DisplayString mText;
		Ogre::StringVector mLines;
		Ogre::Real mPadding;
		bool mDragging;
		unsigned int mStartingLine;
	};

	/*=============================================================================
	| Custom, decorative widget created from a template.
	=============================================================================*/
	class ImageWidget : public Widget
	{
	public:

		// Do not instantiate any widgets directly. Use UIElementsManager.
		ImageWidget(const Ogre::String& name, const Ogre::String& templateName)
		{
			mElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate(templateName, "BorderPanel", name);
		}
	};

	/*=============================================================================
	| Basic slider widget.
	=============================================================================*/
	class Bar : public Widget
	{
	public:

		// Do not instantiate any widgets directly. Use SdkTrayManager.
		Bar(const Ogre::String& pre, const Ogre::String& name, Ogre::Real width, Ogre::Real valueBoxWidth, Ogre::Real minValue, Ogre::Real maxValue, unsigned int snaps)
			: mDragOffset(0.0f)
			, mValue(0.0f)
			, mMinValue(0.0f)
			, mMaxValue(0.0f)
			, mInterval(0.0f)
		{
			mFitToContents = false;
			mElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("UI/"+ pre +"HealthBar", "BorderPanel", name);
			mElement->setWidth(width);
			Ogre::OverlayContainer* c = (Ogre::OverlayContainer*)mElement;
			mValueBox = (Ogre::OverlayContainer*)c->getChild(getName() + "/" + pre + "Bar");
			mValueBox->setWidth(valueBoxWidth);
			mValueTextArea = (Ogre::TextAreaOverlayElement*)mValueBox->getChild(mValueBox->getName() + "/" + pre + "ValueText");

			if (width <= 0) mFitToContents = true;
			
			setRange(minValue, maxValue, snaps, false);
		}

		/*-----------------------------------------------------------------------------
		| Sets the minimum value, maximum value, and the number of snapping points.
		-----------------------------------------------------------------------------*/
		void setRange(Ogre::Real minValue, Ogre::Real maxValue, unsigned int snaps, bool notifyListener = true)
		{
			mMinValue = minValue;
			mMaxValue = maxValue;

			if (snaps <= 1 || mMinValue >= mMaxValue)
			{
				mInterval = 0;
				mValue = mMaxValue;
				if (snaps == 1) mValueTextArea->setCaption(Ogre::StringConverter::toString(mMaxValue));
				else mValueTextArea->setCaption("");
			}
			else
			{
				mInterval = (maxValue - minValue) / (snaps - 1);
				setValue(mMaxValue, notifyListener);
			}
		}

		const Ogre::DisplayString& getValueCaption()
		{
			return mValueTextArea->getCaption();
		}

		/*-----------------------------------------------------------------------------
		| You can use this method to manually format how the value is displayed.
		-----------------------------------------------------------------------------*/
		void setValueCaption(const Ogre::DisplayString& caption)
		{
			mValueTextArea->setCaption(caption);
		}

		void setValue(Ogre::Real value, Ogre::Real valueBoxWidth, bool notifyListener = true)
		{
			//if (mInterval == 0) return;

			mValue = value;// Ogre::Math::Clamp<Ogre::Real>(value, mMinValue, mMaxValue);

			setValueCaption(Ogre::StringConverter::toString(mValue));
			mValueBox->setWidth(valueBoxWidth);
		}

		Ogre::Real getValue()
		{
			return mValue;
		}


	protected:

		/*-----------------------------------------------------------------------------
		| Internal method - given a percentage (from left to right), gets the
		| value of the nearest marker.
		-----------------------------------------------------------------------------*/
		Ogre::Real getSnappedValue(Ogre::Real percentage)
		{
			percentage = Ogre::Math::Clamp<Ogre::Real>(percentage, 0, 1);
			unsigned int whichMarker = (unsigned int)(percentage * (mMaxValue - mMinValue) / mInterval + 0.5);
			return whichMarker * mInterval + mMinValue;
		}

		Ogre::TextAreaOverlayElement* mValueTextArea;
		Ogre::OverlayContainer* mValueBox;
		bool mFitToContents;
		Ogre::Real mDragOffset;
		Ogre::Real mValue;
		Ogre::Real mMinValue;
		Ogre::Real mMaxValue;
		Ogre::Real mInterval;
	};

	/*=============================================================================
	| Basic slider widget.
	=============================================================================*/
	enum Locator {
		Sister = 0,
		Quest
	};
	
	class MiniMap : public Widget
	{
	public:

		// Do not instantiate any widgets directly. Use SdkTrayManager.
		MiniMap(const Ogre::String& name, Ogre::Real width, Ogre::Real minValue, Ogre::Real maxValue)
			: mDragOffset(0.0f)
			, mValueSister(0.0f)
			, mMinValue(0.0f)
			, mMaxValue(0.0f)
			, mInterval(0.0f)
		{
			mFitToContents = false;
			mElement = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("UI/MiniMap", "BorderPanel", name);
			mElement->setWidth(width);
			Ogre::OverlayContainer* c = (Ogre::OverlayContainer*)mElement;
			mLocatorSister = (Ogre::OverlayContainer*)c->getChild(getName() + "/LocatorSister");
			mLocatorSister->setPosition(width / 2, 0);
			
			mLocatorQuest = (Ogre::OverlayContainer*)c->getChild(getName() + "/LocatorQuest");
			mLocatorQuest->setPosition(width / 2, 0);

			if (width <= 0) mFitToContents = true;

			setRange(minValue, maxValue);
		}

		/*-----------------------------------------------------------------------------
		| Sets the minimum value, maximum value, and the number of snapping points.
		-----------------------------------------------------------------------------*/
		void setRange(Ogre::Real minValue, Ogre::Real maxValue)
		{
			mMinValue = minValue;
			mMaxValue = maxValue;

			setValueSister(0, 128);
			setValueQuest(0, 128);
		}

		void setValueSister(Ogre::Real value, Ogre::Real positionLocator)
		{
			mValueSister = Ogre::Math::Clamp<Ogre::Real>(value, mMinValue, mMaxValue);

			mLocatorSister->setPosition(positionLocator - (mLocatorSister->getWidth() / 2.0f), 0);
		}

		void setValueQuest(Ogre::Real value, Ogre::Real positionLocator)
		{
			mValueQuest = Ogre::Math::Clamp<Ogre::Real>(value, mMinValue, mMaxValue);

			mLocatorQuest->setPosition(positionLocator - (mLocatorQuest->getWidth() / 2.0f), 0);
		}

		Ogre::Real getValueSister()
		{
			return mValueSister;
		}

		Ogre::Real getValueQuest()
		{
			return mValueQuest;
		}

		Ogre::OverlayContainer* getQuestLocator() {
			return mLocatorQuest;
		}

	protected:

		/*-----------------------------------------------------------------------------
		| Internal method - given a percentage (from left to right), gets the
		| value of the nearest marker.
		-----------------------------------------------------------------------------*/
		Ogre::Real getSnappedValue(Ogre::Real percentage)
		{
			percentage = Ogre::Math::Clamp<Ogre::Real>(percentage, 0, 1);
			unsigned int whichMarker = (unsigned int)(percentage * (mMaxValue - mMinValue) / mInterval + 0.5);
			return whichMarker * mInterval + mMinValue;
		}

		bool mFitToContents;
		Ogre::Real mDragOffset;
		Ogre::Real mValueSister;
		Ogre::Real mValueQuest;
		Ogre::Real mMinValue;
		Ogre::Real mMaxValue;
		Ogre::Real mInterval;

		Ogre::OverlayContainer* mLocatorSister;
		Ogre::OverlayContainer* mLocatorQuest;
	};

	/*=============================================================================
	| Main class to manage a cursor, backdrop, trays and widgets.
	=============================================================================*/
	class UIElementsManager 
	{
	public:

		/*-----------------------------------------------------------------------------
		| Creates backdrop and trays.
		-----------------------------------------------------------------------------*/
		UIElementsManager(const Ogre::String& name, Ogre::RenderWindow* window) :
			mName(name), mWindow(window), mWidgetDeathRow(), mWidgetPadding(8),
			mWidgetSpacing(2), mTrayPadding(0), mTrayDrag(false), mCursorWasVisible(false),
			mGroupInitProportion(0.0f), mGroupLoadProportion(0.0f), mLoadInc(0.0f)
		{
			mTimer = Ogre::Root::getSingleton().getTimer();
			mLastStatUpdateTime = 0;

			Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

			Ogre::String nameBase = mName + "/";
			std::replace(nameBase.begin(), nameBase.end(), ' ', '_');

			// create overlay layers for everything
			mBackdropLayer = om.create(nameBase + "BackdropLayer");
			mTraysLayer = om.create(nameBase + "WidgetsLayer");
			mPriorityLayer = om.create(nameBase + "PriorityLayer");
			mBackdropLayer->setZOrder(100);
			mTraysLayer->setZOrder(200);
			mPriorityLayer->setZOrder(300);

			// make backdrop and cursor overlay containers
			mBackdrop = (Ogre::OverlayContainer*)om.createOverlayElement("Panel", nameBase + "Backdrop");
			mBackdropLayer->add2D(mBackdrop);
			mDialogShade = (Ogre::OverlayContainer*)om.createOverlayElement("Panel", nameBase + "DialogShade");
			mDialogShade->setMaterialName("SdkTrays/Shade");
			mDialogShade->hide();
			mPriorityLayer->add2D(mDialogShade);

			Ogre::String trayNames[] =
			{ "TopLeft", "Top", "TopRight", "Left", "Center", "Right", "BottomLeft", "Bottom", "BottomRight" };

			for (unsigned int i = 0; i < 9; i++)    // make the real trays
			{
				mTrays[i] = (Ogre::OverlayContainer*)om.createOverlayElementFromTemplate
				("UI/Tray", "BorderPanel", nameBase + trayNames[i] + "Tray");
				mTraysLayer->add2D(mTrays[i]);

				mTrayWidgetAlign[i] = Ogre::GHA_CENTER;

				// align trays based on location
				if (i == TOP || i == CENTER || i == BOTTOM) mTrays[i]->setHorizontalAlignment(Ogre::GHA_CENTER);
				if (i == LEFT || i == CENTER || i == RIGHT) mTrays[i]->setVerticalAlignment(Ogre::GVA_CENTER);
				if (i == TOPRIGHT || i == RIGHT || i == BOTTOMRIGHT) mTrays[i]->setHorizontalAlignment(Ogre::GHA_RIGHT);
				if (i == BOTTOMLEFT || i == BOTTOM || i == BOTTOMRIGHT) mTrays[i]->setVerticalAlignment(Ogre::GVA_BOTTOM);
			}

			// create the null tray for free-floating widgets
			mTrays[9] = (Ogre::OverlayContainer*)om.createOverlayElement("Panel", nameBase + "NullTray");
			mTrayWidgetAlign[9] = Ogre::GHA_LEFT;
			mTraysLayer->add2D(mTrays[9]);
			adjustTrays();

			showTrays();
		}

		/*-----------------------------------------------------------------------------
		| Destroys background, cursor, widgets, and trays.
		-----------------------------------------------------------------------------*/
		virtual ~UIElementsManager()
		{
			Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

			destroyAllWidgets();

			for (unsigned int i = 0; i < mWidgetDeathRow.size(); i++)   // delete widgets queued for destruction
			{
				delete mWidgetDeathRow[i];
			}
			mWidgetDeathRow.clear();

			om.destroy(mBackdropLayer);
			om.destroy(mTraysLayer);
			om.destroy(mPriorityLayer);

			Widget::nukeOverlayElement(mBackdrop);
			Widget::nukeOverlayElement(mDialogShade);

			for (unsigned int i = 0; i < 10; i++)
			{
				Widget::nukeOverlayElement(mTrays[i]);
			}
		}

		/*-----------------------------------------------------------------------------
		| Converts a 2D screen coordinate (in pixels) to a 3D ray into the scene.
		-----------------------------------------------------------------------------*/
		static Ogre::Ray screenToScene(Ogre::Camera* cam, const Ogre::Vector2& pt)
		{
			return cam->getCameraToViewportRay(pt.x, pt.y);
		}

		/*-----------------------------------------------------------------------------
		| Converts a 3D scene position to a 2D screen position (in relative screen size, 0.0-1.0).
		-----------------------------------------------------------------------------*/
		static Ogre::Vector2 sceneToScreen(Ogre::Camera* cam, const Ogre::Vector3& pt)
		{
			Ogre::Vector3 result = cam->getProjectionMatrix() * cam->getViewMatrix() * pt;
			return Ogre::Vector2((result.x + 1) / 2, (-result.y + 1) / 2);
		}

		// these methods get the underlying overlays and overlay elements

		Ogre::OverlayContainer* getTrayContainer(AnchorLocation trayLoc) { return mTrays[trayLoc]; }
		Ogre::Overlay* getBackdropLayer() { return mBackdropLayer; }
		Ogre::Overlay* getTraysLayer() { return mTraysLayer; }
		Ogre::OverlayContainer* getBackdropContainer() { return mBackdrop; }

		void showAll()
		{
			showBackdrop();
			showTrays();
		}

		void hideAll()
		{
			hideBackdrop();
			hideTrays();
		}

		/*-----------------------------------------------------------------------------
		| Displays specified material on backdrop, or the last material used if
		| none specified. Good for pause menus like in the browser.
		-----------------------------------------------------------------------------*/
		void showBackdrop(const Ogre::String& materialName = Ogre::StringUtil::BLANK)
		{
			if (materialName != Ogre::StringUtil::BLANK) mBackdrop->setMaterialName(materialName);
			mBackdropLayer->show();
		}

		void hideBackdrop()
		{
			mBackdropLayer->hide();
		}

		void showTrays()
		{
			mTraysLayer->show();
			mPriorityLayer->show();
		}

		void hideTrays()
		{
			mTraysLayer->hide();
			mPriorityLayer->hide();

			// give widgets a chance to reset in case they're in the middle of something
			for (unsigned int i = 0; i < 10; i++)
			{
				for (unsigned int j = 0; j < mWidgets[i].size(); j++)
				{
					mWidgets[i][j]->_focusLost();
				}
			}
		}

		bool isBackdropVisible() { return mBackdropLayer->isVisible(); }
		bool areTraysVisible() { return mTraysLayer->isVisible(); }

		/*-----------------------------------------------------------------------------
		| Sets horizontal alignment of a tray's contents.
		-----------------------------------------------------------------------------*/
		void setTrayWidgetAlignment(AnchorLocation trayLoc, Ogre::GuiHorizontalAlignment gha)
		{
			mTrayWidgetAlign[trayLoc] = gha;

			for (unsigned int i = 0; i < mWidgets[trayLoc].size(); i++)
			{
				mWidgets[trayLoc][i]->getOverlayElement()->setHorizontalAlignment(gha);
			}
		}

		// padding and spacing methods

		void setWidgetPadding(Ogre::Real padding)
		{
			mWidgetPadding = std::max<int>((int)padding, 0);
			adjustTrays();
		}

		void setWidgetSpacing(Ogre::Real spacing)
		{
			mWidgetSpacing = std::max<int>((int)spacing, 0);
			adjustTrays();
		}
		void setTrayPadding(Ogre::Real padding)
		{
			mTrayPadding = std::max<int>((int)padding, 0);
			adjustTrays();
		}

		virtual Ogre::Real getWidgetPadding() const { return mWidgetPadding; }
		virtual Ogre::Real getWidgetSpacing() const { return mWidgetSpacing; }
		virtual Ogre::Real getTrayPadding() const { return mTrayPadding; }

		/*-----------------------------------------------------------------------------
		| Fits trays to their contents and snaps them to their anchor locations.
		-----------------------------------------------------------------------------*/
		virtual void adjustTrays()
		{
			for (unsigned int i = 0; i < 9; i++)   // resizes and hides trays if necessary
			{
				Ogre::Real trayWidth = 0;
				Ogre::Real trayHeight = mWidgetPadding;
				std::vector<Ogre::OverlayElement*> labelsAndSeps;

				if (mWidgets[i].empty())   // hide tray if empty
				{
					mTrays[i]->hide();
					continue;
				}
				else mTrays[i]->show();

				// arrange widgets and calculate final tray size and position
				for (unsigned int j = 0; j < mWidgets[i].size(); j++)
				{
					Ogre::OverlayElement* e = mWidgets[i][j]->getOverlayElement();

					if (j != 0) trayHeight += mWidgetSpacing;   // don't space first widget

					e->setVerticalAlignment(Ogre::GVA_TOP);
					e->setTop(trayHeight);

					switch (e->getHorizontalAlignment())
					{
					case Ogre::GHA_LEFT:
						e->setLeft(mWidgetPadding);
						break;
					case Ogre::GHA_RIGHT:
						e->setLeft(-(e->getWidth() + mWidgetPadding));
						break;
					default:
						e->setLeft(-(e->getWidth() / 2));
					}

					// prevents some weird texture filtering problems (just some)
					e->setPosition((int)e->getLeft(), (int)e->getTop());
					e->setDimensions((int)e->getWidth(), (int)e->getHeight());

					trayHeight += e->getHeight();

					if (e->getWidth() > trayWidth) trayWidth = e->getWidth();
				}

				// add paddings and resize trays
				mTrays[i]->setWidth(trayWidth + 2 * mWidgetPadding);
				mTrays[i]->setHeight(trayHeight + mWidgetPadding);

				for (unsigned int j = 0; j < labelsAndSeps.size(); j++)
				{
					labelsAndSeps[j]->setWidth((int)trayWidth);
					labelsAndSeps[j]->setLeft(-(int)(trayWidth / 2));
				}
			}

			for (unsigned int i = 0; i < 9; i++)    // snap trays to anchors
			{
				if (i == TOPLEFT || i == LEFT || i == BOTTOMLEFT)
					mTrays[i]->setLeft(mTrayPadding);
				if (i == TOP || i == CENTER || i == BOTTOM)
					mTrays[i]->setLeft(-mTrays[i]->getWidth() / 2);
				if (i == TOPRIGHT || i == RIGHT || i == BOTTOMRIGHT)
					mTrays[i]->setLeft(-(mTrays[i]->getWidth() + mTrayPadding));

				if (i == TOPLEFT || i == TOP || i == TOPRIGHT)
					mTrays[i]->setTop(mTrayPadding);
				if (i == LEFT || i == CENTER || i == RIGHT)
					mTrays[i]->setTop(-mTrays[i]->getHeight() / 2);
				if (i == BOTTOMLEFT || i == BOTTOM || i == BOTTOMRIGHT)
					mTrays[i]->setTop(-mTrays[i]->getHeight() - mTrayPadding);

				// prevents some weird texture filtering problems (just some)
				mTrays[i]->setPosition((int)mTrays[i]->getLeft(), (int)mTrays[i]->getTop());
				mTrays[i]->setDimensions((int)mTrays[i]->getWidth(), (int)mTrays[i]->getHeight());
			}
		}

		HUDText* createHUDText(AnchorLocation trayLoc, const Ogre::String& name, const Ogre::DisplayString& caption,
			Ogre::Real width, Ogre::Real height)
		{
			HUDText* tb = new HUDText(name, caption, width, height);
			moveWidgetToTray(tb, trayLoc);
			//tb->_assignListener(mListener);
			return tb;
		}

		DialogTextBox* createDialogTextBox(AnchorLocation trayLoc, const Ogre::String& name, const Ogre::DisplayString& caption,
			Ogre::Real width, Ogre::Real height)
		{
			DialogTextBox* tb = new DialogTextBox(name, caption, width, height);
			moveWidgetToTray(tb, trayLoc);
			//tb->_assignListener(mListener);
			return tb;
		}

		ImageWidget* createDecorWidget(AnchorLocation trayLoc, const Ogre::String& name, const Ogre::String& templateName)
		{
			ImageWidget* dw = new ImageWidget(name, templateName);
			moveWidgetToTray(dw, trayLoc);
			return dw;
		}

		Bar* createHealthBar(AnchorLocation trayLoc, const Ogre::String& pre, const Ogre::String& name, Ogre::Real width,
			Ogre::Real valueBoxWidth, Ogre::Real minValue, Ogre::Real maxValue, unsigned int snaps)
		{
			Bar* tb = new Bar(pre, name, width, valueBoxWidth, minValue, maxValue, snaps);
			moveWidgetToTray(tb, trayLoc);
			//tb->_assignListener(mListener);
			return tb;
		}

		MiniMap* createMiniMap(AnchorLocation trayLoc, const Ogre::String& name, Ogre::Real width,
			Ogre::Real minValue, Ogre::Real maxValue)
		{
			MiniMap* tb = new MiniMap(name, width, minValue, maxValue);
			moveWidgetToTray(tb, trayLoc);
			//tb->_assignListener(mListener);
			return tb;
		}

		/* 
		/*-----------------------------------------------------------------------------
		| Shows logo in the specified location.
		-----------------------------------------------------------------------------#1#
		void showLogo(AnchorLocation trayLoc, int place = -1)
		{
			if (!isLogoVisible()) mLogo = createDecorWidget(NONE, mName + "/Logo", "SdkTrays/Logo");
			moveWidgetToTray(mLogo, trayLoc, place);
		}

		void hideLogo()
		{
			if (isLogoVisible())
			{
				destroyWidget(mLogo);
				mLogo = 0;
			}
		}

		bool isLogoVisible()
		{
			return mLogo != 0;
		}*/

		/*-----------------------------------------------------------------------------
		| Gets a widget from a tray by place.
		-----------------------------------------------------------------------------*/
		Widget* getWidget(AnchorLocation trayLoc, unsigned int place)
		{
			if (place < mWidgets[trayLoc].size()) return mWidgets[trayLoc][place];
			return 0;
		}

		/*-----------------------------------------------------------------------------
		| Gets a widget from a tray by name.
		-----------------------------------------------------------------------------*/
		Widget* getWidget(AnchorLocation trayLoc, const Ogre::String& name)
		{
			for (unsigned int i = 0; i < mWidgets[trayLoc].size(); i++)
			{
				if (mWidgets[trayLoc][i]->getName() == name) return mWidgets[trayLoc][i];
			}
			return 0;
		}

		/*-----------------------------------------------------------------------------
		| Gets a widget by name.
		-----------------------------------------------------------------------------*/
		Widget* getWidget(const Ogre::String& name)
		{
			for (unsigned int i = 0; i < 10; i++)
			{
				for (unsigned int j = 0; j < mWidgets[i].size(); j++)
				{
					if (mWidgets[i][j]->getName() == name) return mWidgets[i][j];
				}
			}
			return 0;
		}

		/*-----------------------------------------------------------------------------
		| Gets the number of widgets in total.
		-----------------------------------------------------------------------------*/
		unsigned int getNumWidgets()
		{
			unsigned int total = 0;

			for (unsigned int i = 0; i < 10; i++)
			{
				total += mWidgets[i].size();
			}

			return total;
		}

		/*-----------------------------------------------------------------------------
		| Gets the number of widgets in a tray.
		-----------------------------------------------------------------------------*/
		size_t getNumWidgets(AnchorLocation trayLoc)
		{
			return mWidgets[trayLoc].size();
		}

		/*-----------------------------------------------------------------------------
		| Gets all the widgets of a specific tray.
		-----------------------------------------------------------------------------*/
		WidgetIterator getWidgetIterator(AnchorLocation trayLoc)
		{
			return WidgetIterator(mWidgets[trayLoc].begin(), mWidgets[trayLoc].end());
		}

		/*-----------------------------------------------------------------------------
		| Gets a widget's position in its tray.
		-----------------------------------------------------------------------------*/
		int locateWidgetInTray(Widget* widget)
		{
			for (unsigned int i = 0; i < mWidgets[widget->getTrayLocation()].size(); i++)
			{
				if (mWidgets[widget->getTrayLocation()][i] == widget) return i;
			}
			return -1;
		}

		/*-----------------------------------------------------------------------------
		| Destroys a widget.
		-----------------------------------------------------------------------------*/
		void destroyWidget(Widget* widget)
		{
			if (!widget) OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Widget does not exist.", "TrayManager::destroyWidget");

			mTrays[widget->getTrayLocation()]->removeChild(widget->getName());

			WidgetList& wList = mWidgets[widget->getTrayLocation()];
			wList.erase(std::find(wList.begin(), wList.end(), widget));

			widget->cleanup();

			mWidgetDeathRow.push_back(widget);

			adjustTrays();
		}

		void destroyWidget(AnchorLocation trayLoc, unsigned int place)
		{
			destroyWidget(getWidget(trayLoc, place));
		}

		void destroyWidget(AnchorLocation trayLoc, const Ogre::String& name)
		{
			destroyWidget(getWidget(trayLoc, name));
		}

		void destroyWidget(const Ogre::String& name)
		{
			destroyWidget(getWidget(name));
		}

		/*-----------------------------------------------------------------------------
		| Destroys all widgets in a tray.
		-----------------------------------------------------------------------------*/
		void destroyAllWidgetsInTray(AnchorLocation trayLoc)
		{
			while (!mWidgets[trayLoc].empty()) destroyWidget(mWidgets[trayLoc][0]);
		}

		/*-----------------------------------------------------------------------------
		| Destroys all widgets.
		-----------------------------------------------------------------------------*/
		void destroyAllWidgets()
		{
			for (unsigned int i = 0; i < 10; i++)  // destroy every widget in every tray (including null tray)
			{
				destroyAllWidgetsInTray((AnchorLocation)i);
			}
		}

		/*-----------------------------------------------------------------------------
		| Adds a widget to a specified tray.
		-----------------------------------------------------------------------------*/
		void moveWidgetToTray(Widget* widget, AnchorLocation trayLoc, int place = -1)
		{
			if (!widget) OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Widget does not exist.", "TrayManager::moveWidgetToTray");

			// remove widget from old tray
			WidgetList& wList = mWidgets[widget->getTrayLocation()];
			WidgetList::iterator it = std::find(wList.begin(), wList.end(), widget);
			if (it != wList.end())
			{
				wList.erase(it);
				mTrays[widget->getTrayLocation()]->removeChild(widget->getName());
			}

			// insert widget into new tray at given position, or at the end if unspecified or invalid
			if (place == -1 || place > (int)mWidgets[trayLoc].size()) place = (int)mWidgets[trayLoc].size();
			mWidgets[trayLoc].insert(mWidgets[trayLoc].begin() + place, widget);
			mTrays[trayLoc]->addChild(widget->getOverlayElement());

			widget->getOverlayElement()->setHorizontalAlignment(mTrayWidgetAlign[trayLoc]);

			// adjust trays if necessary
			if (widget->getTrayLocation() != NONE || trayLoc != NONE) adjustTrays();

			widget->_assignToTray(trayLoc);
		}

		void moveWidgetToTray(const Ogre::String& name, AnchorLocation trayLoc, unsigned int place = -1)
		{
			moveWidgetToTray(getWidget(name), trayLoc, place);
		}

		void moveWidgetToTray(AnchorLocation currentTrayLoc, const Ogre::String& name, AnchorLocation targetTrayLoc,
			int place = -1)
		{
			moveWidgetToTray(getWidget(currentTrayLoc, name), targetTrayLoc, place);
		}

		void moveWidgetToTray(AnchorLocation currentTrayLoc, unsigned int currentPlace, AnchorLocation targetTrayLoc,
			int targetPlace = -1)
		{
			moveWidgetToTray(getWidget(currentTrayLoc, currentPlace), targetTrayLoc, targetPlace);
		}

		/*-----------------------------------------------------------------------------
		| Removes a widget from its tray. Same as moving it to the null tray.
		-----------------------------------------------------------------------------*/
		void removeWidgetFromTray(Widget* widget)
		{
			moveWidgetToTray(widget, NONE);
		}

		void removeWidgetFromTray(const Ogre::String& name)
		{
			removeWidgetFromTray(getWidget(name));
		}

		void removeWidgetFromTray(AnchorLocation trayLoc, const Ogre::String& name)
		{
			removeWidgetFromTray(getWidget(trayLoc, name));
		}

		void removeWidgetFromTray(AnchorLocation trayLoc, int place)
		{
			removeWidgetFromTray(getWidget(trayLoc, place));
		}

		/*-----------------------------------------------------------------------------
		| Removes all widgets from a widget tray.
		-----------------------------------------------------------------------------*/
		void clearTray(AnchorLocation trayLoc)
		{
			if (trayLoc == NONE) return;      // can't clear the null tray

			while (!mWidgets[trayLoc].empty())   // remove every widget from given tray
			{
				removeWidgetFromTray(mWidgets[trayLoc][0]);
			}
		}

		/*-----------------------------------------------------------------------------
		| Removes all widgets from all widget trays.
		-----------------------------------------------------------------------------*/
		void clearAllTrays()
		{
			for (unsigned int i = 0; i < 9; i++)
			{
				clearTray((AnchorLocation)i);
			}
		}

		/*-----------------------------------------------------------------------------
		| Process frame events. Updates frame statistics widget set and deletes
		| all widgets queued for destruction.
		-----------------------------------------------------------------------------*/
		bool frameRenderingQueued(const Ogre::FrameEvent& evt)
		{
			for (unsigned int i = 0; i < mWidgetDeathRow.size(); i++)
			{
				delete mWidgetDeathRow[i];
			}
			mWidgetDeathRow.clear();


			unsigned long currentTime = mTimer->getMilliseconds();

			return true;
		}

		void windowUpdate()
		{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS && OGRE_PLATFORM != OGRE_PLATFORM_NACL
			mWindow->update();
#endif
		}
	

	protected:

		Ogre::String mName;                   // name of this tray system
		Ogre::RenderWindow* mWindow;          // render window
		Ogre::Overlay* mBackdropLayer;        // backdrop layer
		Ogre::Overlay* mTraysLayer;           // widget layer
		Ogre::Overlay* mPriorityLayer;        // top priority layer
		Ogre::OverlayContainer* mBackdrop;    // backdrop
		Ogre::OverlayContainer* mTrays[10];   // widget trays
		WidgetList mWidgets[10];              // widgets
		WidgetList mWidgetDeathRow;           // widget queue for deletion
		Ogre::Real mWidgetPadding;            // widget padding
		Ogre::Real mWidgetSpacing;            // widget spacing
		Ogre::Real mTrayPadding;              // tray padding
		bool mTrayDrag;                       // a mouse press was initiated on a tray
		Ogre::OverlayContainer* mDialogShade; // top priority talk shade
		bool mCursorWasVisible;               // cursor state before showing talk
		Ogre::Real mGroupInitProportion;      // proportion of load job assigned to initialising one resource group
		Ogre::Real mGroupLoadProportion;      // proportion of load job assigned to loading one resource group
		Ogre::Real mLoadInc;                  // loading increment
		Ogre::GuiHorizontalAlignment mTrayWidgetAlign[10];   // tray widget alignments
		Ogre::Timer* mTimer;                  // Root::getSingleton().getTimer()
		unsigned long mLastStatUpdateTime;    // The last time the stat text were updated
	};

}

#endif