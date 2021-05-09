## videoEdit

débuté à Embrun le 28/11/19

## 24 Janvier 2020

-ges-launch a l'air de prendre les .MOV de dji en +clip si je fais de l'output rendering (i.e. "-o out.mp4 -f "video/quicktime:video/x-h264"")
	je me suis pris la tête archi longtemps parce que je croyais que ça ne marchait pas. En fait quand je converti en h264/mp4 comme ça ges-launch ne donne que le premier clip en output dans une timeline:
	gst-launch-1.0 filesrc location=DJI_0174.MOV ! qtdemux ! vaapidecodebin ! vaapipostproc ! video/x-raw,height=1080 ! vaapih264enc ! qtmux ! filesink location=174.mp4
	Je convertissait en raw dans du mp4 mais prenait énormément de place:
	gst-launch-1.0 filesrc location=DJI_0174.MOV ! qtdemux ! vaapidecodebin ! vaapipostproc ! video/x-raw,height=1080 ! qtmux ! filesink location=174.mp4
	Au final pourquoi ça marche avec les .MOV de dji et pas après conversion en height 1080... Une histoire de format???

-gst/gstgtk.c -> visionnage/choix des times de cut (marche sur h264/mp4, mais moins bien que sur du raw)

-title: réglages (ges/LOG_ges)
		



## Biblio/Notions:
https://developer.mozilla.org/fr/docs/Web/HTML/Element/video
https://developer.mozilla.org/en-US/docs/Web/Media/Formats/Video_codecs
http://samples.mplayerhq.hu
EDL (Edit Decision List)
Pixar ont un projet OpenTimelineIO -> pas fouillé



## ges/ gtreamer editing services

## gst_gtk/ gst en GUI GTK+ selon un des tutos gstreamer

## html5_video/
	- mosaic.html grid de travail
	- seek_simple: element <video> récup des infos src et position au onClick() dessus
	- forcer une size WxH arbitraire sans que l'aspect ration force redimensionnement: css object-fit dans size.html
	
## cut/
	- gestion des cuts: dynamique (sélection first click début segment, second click fin segment) -> fait dans cut/cut.html
	
## nodejs ffmpeg (gstreamer???)
	- un essai furtif ffmpeg: aucun pb pour piloter ffmpeg en backend
	
## drag_drop/
	- sortablejs (https://sortablejs.github.io/Sortable/)

## stream
	- UDP, base, en vue de passer sur gstreamer
 




	
	
	
	
	
