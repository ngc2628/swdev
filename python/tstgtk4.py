#!/usr/bin/python

import gi
gi.require_version('Gtk','4.0')
from gi.repository import Gtk

def onActivate(app):
  gtkwin=Gtk.ApplicationWindow(application=app)
  gtkwin.set_title('Hallo Tiger')
  gtkbutton=Gtk.Button(label="Hallo Tiger")
  gtkbutton.connect('clicked', lambda x: gtkwin.close())
  gtkwin.set_child(gtkbutton)
  gtkwin.present()

app=Gtk.Application(application_id='fuzzball.eu')
app.connect('activate',onActivate)
app.run()

