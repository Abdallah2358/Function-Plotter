# [**Function Plotter**](https://github.com/Abdallah2358/Function-Plotter)

## Description:
This is Desktop Application with a simple UI to plot simple functions with support of these operators [ + - * / ^]
Built with QT in C++.


## Getting started
### Requirements and pre-requisites
To effectively initialize this project, user needs to have [Qt 5.12](https://www.qt.io/offline-installers) with the suitable Mingw build kit on their machines and [Qt Creator](https://www.qt.io/offline-installers) to build the project.

To open the project in ``Qt Creator`` simply open the [task1.pro](task1.pro).


### Tests
The tests are written using the [**Qt Test**](https://doc.qt.io/qt-5/qttest-index.html) to test helper function of the project. 

The tests are run by using Qt Creator by opening the tst_className.pro from testing project in the [Test] .

There are provided 7 tests provided in the [**tests.py**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/tasks/tests.py).
Each state has at least 1 test for its case.

### Project Structure
    └───task_state_api
    │   db.sqlite3
    │   manage.py
    │   Pipfile
    │   Pipfile.lock
    │
    ├───config
    │       asgi.py
    │       settings.py
    │       urls.py
    │       wsgi.py
    │       __init__.py
    │
    └───tasks
        │   admin.py
        │   apps.py
        │   models.py
        │   serializers.py
        │   tests.py
        │   urls.py
        │   views.py
        │   __init__.py
        │
        └───migrations
                0001_initial.py
                0002_auto_20211214_1656.py
                0003_auto_20211214_1756.py
                __init__.py
This structure has 2 main folders:
* config, which stores the [**settings**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/config/settings.py) of the django and the admin [**urls**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/config/urls.py).
* tasks, which stores the most important aspects of the api like [**models**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/tasks/models.py), [**views**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/tasks/views.py), [**urls**](https://github.com/MoazSamy/Backend-Task/blob/main/Technical%20project/task_state_api/tasks/urls.py), and more..


## API Reference
### Endpoints
#### GET /api/v1/
* Returns a list of tasks.
* Sample: `curl http://127.0.0.1:8000/api/v1/`
```
      [{
        "id":1,
        "state":"ARCH",
        "title":"Work"
       },
       {
        "id":2,
        "state":"ARCH",
        "title":"School"
       },
       {
        "id":3,
        "state":"DRFT",
        "title":"Chores"
       },
       {
        "id":4,
        "state":"ARCH",
        "title":"Chores"
      }]
```

#### GET /api/v1/<task_id>
* Returns a single task by id.
* Returns a 404:NOT_FOUND if the id doesn't exist.
* Sample: `curl http://127.0.0.1:5000/api/v1/1/`
```
      {
      "id":1,
      "state":"ARCH",
      "title":"Work"
      }
```

#### DELETE /api/v1/<task_id>
* Deletes an existing task using its id.

#### GET /api/v1/<task_id>/advance
* Advances the task to the next stage of progress, using its id.
* This returns a 400:BAD_REQUEST if the task is already archived and can't progress any further.
```
      {
      "id":3,
      "state":"DN",
      "title":"Chores"
      }
```

### GET /api/tasks/<task_id>/archive
* Advances the task to the archived state, using its id.
* This returns a 400:BAD_REQUEST if the task is already archived.
```
      {
      "id":3,
      "state":"ARCH",
      "title":"Chores"
      }
```

### Error handling
The API returns a couple of error messages for invalid action, or for inexistent task:
* `400`: Bad Request
* `404`: Not Found

## Authors
Moaz Samy