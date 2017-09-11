#ifndef STATEHELPER_H
#define STATEHELPER_H

//this class assists in the tracking of state transitions including its initialiation and cleanup
template <class T>
class StateHelper
{
public:
	StateHelper();
	
	bool IsChangingState() const;
	bool IsInitialized() const;
	bool IsCleanedUp() const;
	bool IsInSubState() const;

	T GetCurrentState() const;
	void SetNextState(T nextState, bool needInitalize = false, bool needCleanup = false);
	void SetInitalizingDone();
	void SetCleanUpDone();
	void SetSubState();

	void CheckAndSwapState();

private:
	void SetCurrentStateFromNextState();

	T mCurrentState;
	T mNextState;
	bool mIsInitalized; //this is used to indicate if this current state has gone through the initalization step.
	bool mIsCleanedUp; //this is used to indicate if this current state has gone through cleanup step before a state change is done.
	bool mIsInSubState; //this is used to indicate if this current state has its initalization or cleanup initiated.
	bool mSkipNextInitialize; //skips the next initialie after the state change
};

template<typename T>
inline StateHelper<T>::StateHelper()
	:mCurrentState((T)-1)
	, mNextState((T)-1)
	, mIsInitalized(false)
	, mIsCleanedUp(false)
	, mIsInSubState(false)
	, mSkipNextInitialize(false)
{
}

template<typename T>
inline bool StateHelper<T>::IsChangingState() const
{
	return mCurrentState != mNextState;
}

template<typename T>
inline bool StateHelper<T>::IsInitialized() const
{
	return mIsInitalized;
}

template<typename T>
inline bool StateHelper<T>::IsCleanedUp() const
{
	return mIsCleanedUp;
}

template<typename T>
inline bool StateHelper<T>::IsInSubState() const
{
	return mIsInSubState;
}

template<typename T>
inline T StateHelper<T>::GetCurrentState() const
{
	return mCurrentState;
}

template<typename T>
inline void StateHelper<T>::SetNextState(T nextState, bool needInitalize, bool needCleanup)
{
	mSkipNextInitialize = !needInitalize;

	if (!needCleanup)
	{
		SetCleanUpDone();		
	}

	mNextState = nextState;
}

template<typename T>
inline void StateHelper<T>::SetInitalizingDone()
{
	mIsInitalized = true;
	mIsInSubState = false;
}

template<typename T>
inline void StateHelper<T>::SetCleanUpDone()
{
	mIsCleanedUp = true;
	mIsInSubState = false;
}

template<typename T>
inline void StateHelper<T>::SetSubState()
{
	mIsInSubState = true;
}

template<class T>
inline void StateHelper<T>::CheckAndSwapState()
{
	if (IsCleanedUp() && IsChangingState())
		SetCurrentStateFromNextState();
}

template<typename T>
inline void StateHelper<T>::SetCurrentStateFromNextState()
{
	mCurrentState = mNextState;

	if (mSkipNextInitialize)
	{
		SetInitalizingDone();
		mSkipNextInitialize = false;
	}
}

#endif