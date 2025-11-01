import socket
import time
import AviationWeatherServer
import light_control
try:
    import config
except ImportError:
    class config:  # type: ignore
        REFRESH_MINUTES = 15

def run_server():
    # Create a socket object
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to a specific IP and port
    s.bind(('', 80))

    # Listen for incoming connections
    s.listen(5)


    last_action_time = time.time()
    status = True

    while True:
        current_time = time.time()
        conn, addr = s.accept()
        print('Got a connection from %s' % str(addr))
        request = conn.recv(1024)
        request = str(request)
        print('Content = %s' % request)

        buttonA = request.find('/?buttonA')
        buttonB = request.find('/?buttonB')

        if buttonA == 6:
            print('Button A was pressed')
            # Add your action here
            status = True
            AviationWeatherServer.AvWeather()
            last_action_time = current_time


        if buttonB == 6:
            print('Button B was pressed')
            # Add your action here
            status = False
            light_control.turnOff()

        current_time = time.time()
        refresh_secs = int(getattr(config, 'REFRESH_MINUTES', 15)) * 60
        if current_time - last_action_time >= refresh_secs and status:
            print('{} minutes have passed, performing action'.format(int(getattr(config, 'REFRESH_MINUTES', 15))))
            # Add your action here
            AviationWeatherServer.AvWeather()
            last_action_time = current_time

        response = """HTTP/1.1 200 OK
    Content-Type: text/html

<!DOCTYPE HTML>
<html>
<head>
    <title>Control Panel</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 50px;
        }
        h1 {
            color: #333;
        }
        button {
            background-color: #4CAF50; /* Green */
            border: none;
            color: white;
            padding: 20px 40px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 20px;
            margin: 20px 2px;
            cursor: pointer;
            border-radius: 12px;
            transition-duration: 0.4s;
        }
        button:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <h1>Weather Board Control Panel</h1>
    <button onclick="location.href='/?buttonA'">Activate Weather Service</button>
    <button onclick="location.href='/?buttonB'">Turn Off Lights</button>
</body>
</html>

    """
        conn.send(response)
        conn.close()