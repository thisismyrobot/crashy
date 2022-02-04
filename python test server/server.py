import flask


app = flask.Flask(__name__)


@app.route('/upload', methods=['POST'])
def upload():
    # Just enough to save a file, do not use this as a good example...
    file = flask.request.files['image'].save('uploaded.jpg')
    return 'Uploaded!'


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
