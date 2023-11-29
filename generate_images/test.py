import base64
from io import BytesIO
from PIL import Image

def svg_to_array(svg_content):
    image_data = []
    for line in svg_content.splitlines():
        if 'href="data:image/png;base64' in line:
            start_index = line.find('base64,') + len('base64,')
            end_index = line.find('"', start_index)
            image_data.append(line[start_index:end_index])

    images = [Image.open(BytesIO(base64.b64decode(data))) for data in image_data]
    images_bw = [image.convert('1') for image in images]

    pixel_data = []
    for col in range(images_bw[0].width):
        for row in range(images_bw[0].height):
            for image_bw in images_bw:
                pixel_data.append(1 if image_bw.getpixel((col, row)) == 0 else 0)

    return pixel_data[::-1]  # Reverse the array

# Example SVG content
svg_content = '''
<svg version="1.2" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 15 15" width="15" height="15">
	<title>ok</title>
	<defs>
		<image  width="15" height="15" id="img1" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA8AAAAPAQMAAAABGAcJAAAAAXNSR0IB2cksfwAAAANQTFRF////p8QbyAAAAAxJREFUeJxjYCABAAAALQABxdfAhgAAAABJRU5ErkJggg=="/>
		<image  width="15" height="15" id="img2" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA8AAAAPAQMAAAABGAcJAAAAAXNSR0IB2cksfwAAAAZQTFRF////AAAAVcLTfgAAACxJREFUeJwVyMEJACAMALGAClqXcrSO7AhaOO4RaPSsRtrHPOI/xbUIhav8AWlsBN9VhFVaAAAAAElFTkSuQmCC"/>
	</defs>
	<style>
	</style>
	<use id="Layer 1" href="#img1" x="0" y="0"/>
	<use id="2" href="#img2" x="0" y="2"/>
</svg>
'''

# Generate the array format with newline every 15th number
array_format = svg_to_array(svg_content)

# Print the result with newline every 15th number
print('{', end='\n')
for i, num in enumerate(array_format, start=1):
    print(f'    {num},', end='\n' if i % 15 == 0 and i != len(array_format) else ' ')
print('}')
