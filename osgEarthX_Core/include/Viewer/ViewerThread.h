/**
	@brief ��Ⱦ�߳�
	@author Hao Luo
	@date 2014/11/03
*/

#ifndef OSGEARTHX_CORE_VIEWER_THREAD_H
#define OSGEARTHX_CORE_VIEWER_THREAD_H 1

#include <OpenThreads/Thread>
#include <osgViewer/ViewerBase>

namespace osgEarthXCore
{
	class ViewerThread : public OpenThreads::Thread
	{
	public:
		ViewerThread( osgViewer::ViewerBase* pViewer = NULL );
		virtual ~ViewerThread();

		/**������Ҫ���߳������е�View*/
		void setViewer( osgViewer::ViewerBase* pViewer );

		/**derived from OpenThreads::Thread*/

		//�߳��ڲ�ִ������
		virtual void run();

		//ȡ������
		virtual int cancel();

	protected:
		osgViewer::ViewerBase* m_pViewer;
	};
}

#endif