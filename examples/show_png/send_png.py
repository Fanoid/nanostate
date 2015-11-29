from nanostate import *
from PIL import Image
import base64

parser = argparse.ArgumentParser(description="A nanostate client to send png.")
parser.add_argument('--identity', '-I', dest = "identity", required = True, help = "A string to identiy this client")
parser.add_argument('--connect', '-C', dest = "addr", required = True, help = "The address to bind, which should be compatible with nanomsg, such as 'tcp://127.0.0.1:15000'")
parser.add_argument('--png-filename', dest = "filename", required = True, help = "A png file")

args = parser.parse_args()
print(args)
identity = args.identity
addr = args.addr
filename = args.filename

img = base64.b64encode(open(filename, 'rb').read()).decode('utf-8').replace('\n', '')

nanostate = Nanostate(identity, addr)
while True:
    time.sleep(1)
    nanostate.send_state_update("image", img);
    break
    result, state = nanostate.recv_state_update()
    if result:
        print(identity + " received: " + str(state))
