# 📌 TaskNote

일정(Calendar), 메모(Memo), 할 일(Todo)을 하나로 통합한 데스크톱 애플리케이션
Qt Widgets 기반으로 제작된 멀티 페이지 UI 프로젝트

---

## 📂 Project Structure

```text
TaskNote/
├── src/
│   ├── MainWindow/
│   ├── pages/
│   │   ├── Calendar/
│   │   │   ├──Calendar
│   │   │   ├──CalendarCellWidget
│   │   ├── TodoList/
│   │   │   ├──ToDoList
│   │   ├── Memo/
│   │   │   ├──Memo
│   │   └── Setting/
│   │       └── Setting
│   ├── core/
│   │   ├── PageID
│   │   └── PageManager
│   ├── Data/
│   │   ├── DatabaseManager
│   │   └── ScheduleItem
│   └── resources/
├── CMakeLists.txt
├── main
└── README.md
```

---

## 📖 Directory Description

### 🔹 MainWindow

* 전체 UI의 루트
* QStackedWidget 구조
* 페이지 전환의 중심

### 🔹 pages/

* 각 기능별 UI 페이지 (View 역할)

  * Calendar
  * TodoList
  * Memo
  * Setting

### 🔹 core/        

* **PageManager**

  * 페이지 이동 및 흐름 제어
  * 뒤로가기 / 특정 페이지 이동 처리

* **PageID**

  * PageID enum

### 🔹 data/        

* **DatabaseManager**

  * .sqliteDB 생성 / 연결
  * 일정 추가 / 저장 / 불러오기 / 삭제

* **ScheduleItem**

  * ScheduleItem struct

### 🔹 resources/

* 이미지 리소스

---
## ✨ Features

* 멀티 페이지 구조 (QStackedWidget)
* 모듈 분리 구조 (Pages / Core / Data)

---

## 👨‍💻 Author

* C++ / Qt 기반 데스크톱 애플리케이션 개발
* 구조 설계 및 상태 관리 중심으로 프로젝트 진행

---
