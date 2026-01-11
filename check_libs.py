try:
    import icalendar
    print("icalendar available")
except ImportError:
    print("icalendar not available")

try:
    import vobject
    print("vobject available")
except ImportError:
    print("vobject not available")
