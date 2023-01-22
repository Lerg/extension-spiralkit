#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import os
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
				f.write(
f'''
animations {{
  id: "{sprite['name']}"
  images {{
    image: "{sprite['path']}"
    sprite_trim_mode: SPRITE_TRIM_MODE_OFF
  }}
  playback: PLAYBACK_LOOP_FORWARD
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
if len(atlas_infos) > 0:
	factories = []
	for atlas in atlas_infos:
		factories.append([atlas, f'/assets/sprites/{atlas}.go'])
	f.write(generate_factories('sprites', factories))
if len(label_infos) > 0:
	factories = []
	for label in label_infos:
		label_name = label['name']
		factories.append([label_name, f'/assets/labels/{label_name}.go'])
	f.write(generate_factories('labels', factories))
if len(sound_infos) > 0:
	factories = []
	for sound in sound_infos:
		sound_name = sound['name']
		factories.append([sound_name, f'/assets/sounds/{sound_name}.go'])
	f.write(generate_factories('sounds', factories))
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
		f.write(f'''		static const asset::Sprite {path_to_id(sprite['name'])}("/assets/sprites#{sprite['atlas']}", "{sprite['name']}", {sprite['width']}, {sprite['height']});\n''')
		pass
	f.write('	}\n')
if len(label_infos) > 0:
	f.write('	namespace labels {\n')
	for label in label_infos:
		f.write(f'''		static const asset::Label {path_to_id(label['name'])}("/assets/labels#{label['name']}", {label['height']});\n''')
		pass
	f.write('	}\n')
if len(sound_infos) > 0:
	f.write('	namespace sounds {\n')
	for sound in sound_infos:
		f.write(f'''		static const asset::Sound {path_to_id(sound['name'])}("/assets/sounds#{sound['name']}");\n''')
		pass
	f.write('	}\n')
if len(particlefx_infos) > 0:
	f.write('	namespace particlefx {\n')
	for particlefx in particlefx_infos:
		f.write(f'''		static const asset::ParticleFX {path_to_id(particlefx['name'])}("/assets/particlefx#{particlefx['name']}");\n''')
		pass
	f.write('	}\n')
f.write(
'''}

#endif
'''
)
f.close()
