#include "MainWindow.h"

#include <QQuickView>
#include <QScrollArea>
#include <QAction>
#include <QUrl>
#include <QMenuBar>
#include <QApplication>
#include <QAccessible>
#include <QWindow>

#define USE_SCROLL_AREA 1

namespace
{
void printAccessibilityRecursevily(QAccessibleInterface* item)
{
	auto name = item->text(QAccessible::Name);
	auto rect = item->rect();
	qDebug() << item->object()->metaObject()->className() << name << rect;

	for(int i = 0; i < item->childCount(); i++)
	{
		printAccessibilityRecursevily(item->child(i));
	}
}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("Application");
	setMinimumWidth(300);
	setMinimumHeight(300);
	setAccessibleName("Main Window");

	QWidget* centralWidget = new QWidget(this);
	centralWidget->setAccessibleName("Central Widget");
	setCentralWidget(centralWidget);

	QQuickView* view = new QQuickView();
	view->setSource(QUrl("qrc:///main.qml"));
	auto* windowContainer = QWidget::createWindowContainer(view);
	windowContainer->setAccessibleName("Window Container");
	windowContainer->setMinimumWidth(160);
	windowContainer->setMinimumHeight(240);

#if USE_SCROLL_AREA
	auto ScrollArea = new QScrollArea(centralWidget);
	ScrollArea->setMinimumHeight(200);
	ScrollArea->setMinimumWidth(200);
	ScrollArea->move(20, 40);
	ScrollArea->setObjectName("MyScrollArea");
	ScrollArea->setWidgetResizable(true);
	ScrollArea->setWidget(windowContainer);
#else
	windowContainer->setParent(centralWidget);
	windowContainer->move(30, 60);
#endif


	QAction *action = new QAction("Show accessibility hierarchy", this);
	menuBar()->addAction(action);

	QObject::connect(action, &QAction::triggered, [&]() {
		auto list = QApplication::topLevelWindows();
		for(auto* window : list)
		{
			auto* accessibleRoot = window->accessibleRoot();
			if(accessibleRoot)
			{
				qDebug() << "Window title: " << window->title();
				printAccessibilityRecursevily(accessibleRoot);
			}
		}
	});

}

MainWindow::~MainWindow()
{
}

