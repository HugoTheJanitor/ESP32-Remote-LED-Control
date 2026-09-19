from flask import Flask, Response, render_template

app = Flask(__name__)

COLORS = ["red", "green", "blue", "yellow"]
leds = {color: 0 for color in COLORS}


@app.route("/")
def index():
    return render_template("index.html", leds=leds)


@app.route("/toggle/<color>")
def toggle(color):
    if color in leds:
        leds[color] = 1 - leds[color]
    return Response("", status=200)


@app.route("/status/<color>")
def status_one(color):
    if color in leds:
        return Response(str(leds[color]), content_type="text/plain")
    return Response("0", content_type="text/plain")


@app.route("/status")
def status_all():
    return Response(
        ",".join(str(leds[color]) for color in COLORS),
        content_type="text/plain",
    )


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
