Nanostate is a framework for convenient high-level inter-process communication.
In the communication network, each process is identified using a string. Processes communicate by sending and/or receiving **states**, which can be considered as a key-value pairs.
Compared to libmview, the greatest merit is the easy support for multiple languages, especially for JavaScript.
More supports can be added in the future to provide more flexibility, such as Java for Android, Object-C for iOS, and more.


----------

To harness nanostate for your convenience, you actually need to use the following 2 (possible 3) parts:

  1. Hub: Forward messages among multiple sockets. 
  2. Hub-WS: Forward messages among multiple sockets and WebSockets. Needed when you want to use nanostate in your browser applications.
  3. App: Your endpoint in applications to communicate with others.

----------

Thanks to the wide language support of nanomsg and msgpack, we could easily implement these parts using a variety of languages. 
Current implementations and their dependencies are listed as follows.

  - C (Hub): nanomsg
  - C (App): All for C (Hub), msgpack
  - Qt (App): All for C (App), and Qt
  - Python (Hub): nanomsg, Cython, nanomsg-python
  - Python (Hub-WS): All for Python (Hub), CherryPy, and ws4py
  - Python (App): All for Python (Hub), msgpack-python
  - JavaScript (App): msgpack
  - Node.js (Hub): nanomsg, argparse
  - Node.js (Hub-WS): All for Node.js (Hub), and ws
  - Node.js (App): TO ADD


----------

Known issues:

  1. If you encounter this warning "UserWarning: Could not load the default wrapper for your platform: cpy, performance may be affected!" when running hub-ws.py, please ensure that nanomsg-python is corrected compiled and linked to libnanomsg.so, and libnanomsg.so can be found when running. Or try the Node.js version of Hub-WS.
  2. Apps communicate by the mean of "state", which is a pair of "name" and "state". Theoritically, the "state" is intended to be a string (in every languages). But actually in JavaScript, any Object can be sent because of poor type check. It will cause C app be crashed.
