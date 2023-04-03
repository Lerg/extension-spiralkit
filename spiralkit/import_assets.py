#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import os
import json
from PIL import Image

assets_src_path = 'assets_src'
sprites_path = 'assets_src/sprites'

cpp_assets_path = 'native/src/app/assets.h'

sprite_infos = []
atlas_infos = []
label_infos = []
sound_infos = []
particlefx_infos = []

remove_special_chars = str.maketrans('.@-', '___')
def path_to_id(path: str) -> str:
	return path.replace('/', '$').translate(remove_special_chars)

def generate_factories(name, factories) -> str:
	result = \
f'''embedded_instances {{
  id: "{name}"
  data: '''
	shift = ''
	for factory in factories:
		factory_name = factory[0]
		factory_path = factory[1]
		result = result + \
fr'''{shift}"embedded_components {{\n"
  "  id: \"{factory_name}\"\n"
  "  type: \"factory\"\n"
  "  data: \"prototype: \\\"{factory_path}\\\"\\n"
  "load_dynamically: false\\n"
  "dynamic_prototype: false\\n"
  "\"\n"
  "  position {{\n"
  "    x: 0.0\n"
  "    y: 0.0\n"
  "    z: 0.0\n"
  "  }}\n"
  "  rotation {{\n"
  "    x: 0.0\n"
  "    y: 0.0\n"
  "    z: 0.0\n"
  "    w: 1.0\n"
  "  }}\n"
  "}}\n"
'''
		shift = '  '
	result = result + \
'''  ""
  position {
    x: 0.0
    y: 0.0
    z: 0.0
  }
  rotation {
    x: 0.0
    y: 0.0
    z: 0.0
    w: 1.0
  }
  scale3 {
    x: 1.0
    y: 1.0
    z: 1.0
  }
}
'''
	return result

def generate_sound_components(name, sounds) -> str:
	result = \
f'''embedded_instances {{
  id: "{name}"
  data: '''
	shift = ''
	for sound in sounds:
		result = result + \
fr'''{shift}"embedded_components {{\n"
  "  id: \"{sound['name']}\"\n"
  "  type: \"sound\"\n"
  "  data: \"sound: \\\"{sound['path']}\\\"\\n"
  "looping: {'1' if sound['is_loop'] else '0'}\\n"
  "group: \\\"master\\\"\\n"
  "gain: {sound['gain']}\\n"
  "pan: 0.0\\n"
  "speed: 1.0\\n"
  "loopcount: 0\\n"
  "\"\n"
  "  position {{\n"
  "    x: 0.0\n"
  "    y: 0.0\n"
  "    z: 0.0\n"
  "  }}\n"
  "  rotation {{\n"
  "    x: 0.0\n"
  "    y: 0.0\n"
  "    z: 0.0\n"
  "    w: 1.0\n"
  "  }}\n"
  "}}\n"
'''
		shift = '  '
	result = result + \
'''  ""
  position {
    x: 0.0
    y: 0.0
    z: 0.0
  }
  rotation {
    x: 0.0
    y: 0.0
    z: 0.0
    w: 1.0
  }
  scale3 {
    x: 1.0
    y: 1.0
    z: 1.0
  }
}
'''
	return result

print('Finding sprites')
# Collect info of sprites
for root, subdirs, files in os.walk(sprites_path):
	for file in files:
		if file.endswith('.png') or file.endswith('.jpg'):
			image_path = os.path.join(root, file)
			print(image_path)
			atlas_and_name = os.path.splitext(image_path.replace(sprites_path + '/', ''))[0]
			atlas, name = atlas_and_name.split('/', 1)
			image = Image.open(image_path)
			width, height = image.size
			sprite_infos.append({'path': '/' + image_path, 'atlas': atlas, 'name': name, 'width': width, 'height': height})
			if atlas not in atlas_infos:
				atlas_infos.append(atlas)
		elif file.endswith('.json'):
			json_path = os.path.join(root, file)
			print(json_path)
			atlas_and_name = os.path.splitext(json_path.replace(sprites_path + '/', ''))[0]
			atlas, name = atlas_and_name.split('/', 1)
			with open(json_path) as json_file:
				sprite_infos.append({'path': '/' + root + '/', 'atlas': atlas, 'name': name, 'width': 0, 'height': 0, 'animation': json.load(json_file)})
			if atlas not in atlas_infos:
				atlas_infos.append(atlas)

print('Finding labels')
# Collect info of labels
for root, subdirs, files in os.walk('assets/labels'):
	for file in files:
		if file.endswith('.go'):
			label_path = os.path.join(root, file)
			print(label_path)
			f = open(label_path, 'r')
			content = f.readlines()
			f.close()
			height = content[5].rsplit(' ', 1)[1].split('.', 1)[0]
			label_infos.append({'name': file.split('.')[0], 'height': height})

print('Finding sounds')
# Collect info of labels
for root, subdirs, files in os.walk('assets/sounds'):
	for file in files:
		if file.endswith('.wav'):
			sound_path = os.path.join(root, file)
			print(sound_path)
			name = file.split('.')[0]
			json_path = os.path.join(root, name + '.json')
			settings = {
				'loop': False,
				'gain': 1.0
			}
			if os.path.exists(json_path):
				with open(json_path) as json_file:
					settings = json.load(json_file)
			sound_infos.append({'name': name, 'path': '/' + sound_path, 'is_loop': settings['loop'], 'gain': settings['gain']})

print('Finding particlefx')
# Collect info of particlefx
for root, subdirs, files in os.walk('assets/particlefx'):
	for file in files:
		if file.endswith('.go'):
			particlefx_path = os.path.join(root, file)
			print(particlefx_path)
			f = open(particlefx_path, 'r')
			content = f.readlines()
			f.close()
			particlefx_infos.append({'name': file.split('.')[0]})

print('Generating atlases')
# Generate Defold atlases
if len(atlas_infos) > 0 :
	for atlas in atlas_infos:
		f = open('assets/' + atlas + '.atlas', 'w')
		for sprite in sprite_infos:
			if sprite['atlas'] == atlas:
				if sprite.get('animation') != None:
					animation = sprite['animation']
					f.write(
f'''animations {{
  id: "{sprite['name']}"'''
					)
					for frame in animation['frames']:
						f.write(
f'''
  images {{
    image: "{os.path.normpath(sprite['path'] + frame)}"
    sprite_trim_mode: SPRITE_TRIM_MODE_OFF
  }}'''
						)
					f.write(
f'''
  playback: {animation.get('playback', 'PLAYBACK_ONCE_FORWARD')}
  fps: {animation.get('fps', 60)}
  flip_horizontal: {animation.get('flip_horizontal', 0)}
  flip_vertical: {animation.get('flip_vertical', 0)}
}}
'''
					)
				else:
					f.write(
f'''animations {{
  id: "{sprite['name']}"
  images {{
    image: "{sprite['path']}"
    sprite_trim_mode: SPRITE_TRIM_MODE_OFF
  }}
  playback: PLAYBACK_ONCE_FORWARD
  fps: 60
  flip_horizontal: 0
  flip_vertical: 0
}}
'''
					)
		f.write(
'''
margin: 0
extrude_borders: 2
inner_padding: 0
'''
		)
		f.close()

print('Generating assets.collection')
f = open('assets/assets.collection', 'w')
f.write('''name: "default"
scale_along_z: 0
''')

sprite_factories = []
for root, subdirs, files in os.walk('assets/sprites'):
	for file in files:
		if file.endswith('.go'):
			name = file.split('.')[0]
			factory_path = '/' + os.path.join(root, file)
			sprite_factories.append([name, factory_path])
if len(sprite_factories) > 0:
	f.write(generate_factories('sprites', sprite_factories))

label_factories = []
for root, subdirs, files in os.walk('assets/labels'):
	for file in files:
		if file.endswith('.go'):
			name = file.split('.')[0]
			factory_path = '/' + os.path.join(root, file)
			label_factories.append([name, factory_path])
if len(label_factories) > 0:
	f.write(generate_factories('labels', label_factories))

if len(sound_infos) > 0:
	f.write(generate_sound_components('sounds', sound_infos))
if len(particlefx_infos) > 0:
	factories = []
	for particlefx in particlefx_infos:
		particlefx_name = particlefx['name']
		factories.append([particlefx_name, f'/assets/particlefx/{particlefx_name}.go'])
	f.write(generate_factories('particlefx', factories))
f.close()


print('Generating assets.h')
# Generate C++ assets.h
f = open(cpp_assets_path, 'w')
f.write(
'''#ifndef APP_ASSETS_H
#define APP_ASSETS_H

#include <spiralkit/asset.h>

using namespace spiralkit;

namespace app::assets {
'''
)
if len(sprite_infos) > 0:
	f.write('	namespace sprites {\n')
	for sprite in sprite_infos:
		f.write(f'''		inline const asset::Sprite {path_to_id(sprite['name'])}("/assets/sprites#{sprite['atlas']}", "{sprite['name']}", {sprite['width']}, {sprite['height']});\n''')
		pass
	f.write('	}\n')
if len(label_infos) > 0:
	f.write('	namespace labels {\n')
	for label in label_infos:
		f.write(f'''		inline const asset::Label {path_to_id(label['name'])}("/assets/labels#{label['name']}", {label['height']});\n''')
		pass
	f.write('	}\n')
if len(sound_infos) > 0:
	f.write('	namespace sounds {\n')
	for sound in sound_infos:
		f.write(f'''		inline const asset::Sound {path_to_id(sound['name'])}("/assets/sounds#{sound['name']}", "{sound['name']}");\n''')
		pass
	f.write('	}\n')
if len(particlefx_infos) > 0:
	f.write('	namespace particlefx {\n')
	for particlefx in particlefx_infos:
		f.write(f'''		inline const asset::ParticleFX {path_to_id(particlefx['name'])}("/assets/particlefx#{particlefx['name']}");\n''')
		pass
	f.write('	}\n')
f.write(
'''}

#endif
'''
)
f.close()
