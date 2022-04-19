import subprocess
import os

# Well, why make.py?   FASTEST thing I could think of.... and will work on ALL Platforms   + the syntax is WAY TOO COOL
# Thanks to Threads that made the process so so fast....  https://stackoverflow.com/a/21406995.... https://stackoverflow.com/a/50255019.... https://stackoverflow.com/a/3131251.... https://stackoverflow.com/a/50670037

# -------- We Need gdown to Downlad from GDrive --------
try:
    import gdown
except (ImportError, ModuleNotFoundError) as err:
    print("gdown not found.... calling pip install gdown\n\n\n")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "gdown"])
    print("\n\n\nHoping that gdown got installed")
    try:
        import gdown
    except ImportError:
        print("Error Still Exists....")




GDRIVE_URL_ID = "1pGGfm0yh6bExzQlu3Da4-NJP86m6r_3s";
EXTERN_ZIP = "amGHOST_Externs.zip" #Same name on drive
EXTERN_DIR = "./extern"
#               ╺┳┓┏━┓╻ ╻┏┓╻╻  ┏━┓┏━┓╺┳┓     ╻ ╻┏┓╻╺━┓╻┏━┓     ┏━┓┏━╸┏┳┓┏━┓╻ ╻┏━╸
#   ╺━╸╺━╸╺━╸    ┃┃┃ ┃┃╻┃┃┗┫┃  ┃ ┃┣━┫ ┃┃     ┃ ┃┃┗┫┏━┛┃┣━┛     ┣┳┛┣╸ ┃┃┃┃ ┃┃┏┛┣╸    ╺━╸╺━╸╺━╸
#               ╺┻┛┗━┛┗┻┛╹ ╹┗━╸┗━┛╹ ╹╺┻┛ ┛   ┗━┛╹ ╹┗━╸╹╹   ┛   ╹┗╸┗━╸╹ ╹┗━┛┗┛ ┗━╸

externs = {
    "vulkan-sdk-lunarg",
    "glew"
}

def download_components():
    print("Downlaoding", EXTERN_ZIP + ", using gdown from GoogleDrive")
    gdown.download("https://drive.google.com/uc?id=" + GDRIVE_URL_ID, EXTERN_ZIP, quiet=False)
    print("\n")

import zipfile
def unzip_components(what, where_to):
    print("Unzipping", EXTERN_ZIP, "\n")
    with zipfile.ZipFile(what, 'r') as zip_ref:
        zip_ref.extractall(where_to)

def remove_zips():
    print("Deleting", EXTERN_ZIP)
    try: os.remove(EXTERN_ZIP)
    except FileNotFoundError: print("We just downloaded ", EXTERN_ZIP, "\nDid your OS delete that on its own?")
    print("\n")

def check_get_components():
    for lib in externs:
        if not os.path.isdir(EXTERN_DIR + "/" + lib):
            download_components()
            unzip_components(EXTERN_ZIP, EXTERN_DIR)
            remove_zips()
            return

#                 ┏━╸┏┳┓┏━┓╻┏ ┏━╸
#    ╺━╸╺━╸╺━╸    ┃  ┃┃┃┣━┫┣┻┓┣╸    ╺━╸╺━╸╺━╸
#                 ┗━╸╹ ╹╹ ╹╹ ╹┗━╸
def check_cmake():
    print("TODO")

def build_cmake():
    should_gen_build_system = True
    if os.path.isdir("build-x64"):
        if os.path.exists("build-x64/CMakeCache.cmake"):
            should_gen_build_system = False
    else:
        os.mkdir("build-x64")

    # Change Dir
    cwd = os.getcwd()
    os.chdir("./build-x64")

    # Gen Build System if needed
    if should_gen_build_system:
        subprocess.check_call(["cmake ", "../"])

    # Finally BUILD   -- [TODO: Should we call check_call or smth ELSE]
    subprocess.run(["cmake", "--build", ".", "--target", "install", "--config", "release"])

    os.chdir(cwd)


def get_submodules():
    print("\nmake.py : `git submodule init`")
    subprocess.run(["git", "submodule", "init"], shell=True)
    print("\nmake.py : `git submodule update --remote`")
    subprocess.run(["git", "submodule", "update", "--remote"], shell=True)


#               ┏┳┓┏━┓╻┏┓╻
#   ╺━╸╺━╸╺━╸   ┃┃┃┣━┫┃┃┗┫   ╺━╸╺━╸╺━╸
#               ╹ ╹╹ ╹╹╹ ╹
if __name__ == "__main__":
    check_get_components();
    build_cmake();
    
    