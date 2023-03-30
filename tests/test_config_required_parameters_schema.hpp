/*|**************************************************************************************
 * SGCT                                                                                  *
 * Simple Graphics Cluster Toolkit                                                       *
 *                                                                                       *
 * Copyright (c) 2012-2023                                                               *
 * For conditions of distribution and use, see copyright notice in LICENSE.md            *
 ****************************************************************************************/

#include <filesystem>
#include <string>

std::filesystem::path schemaDir = std::filesystem::u8path("../");

const std::string sgctSchemaJson = 
R"|({
  "$schema": "http://json-schema.org/draft-07/schema",

  "$defs": {
    "quat": {
      "type": "object",
      "anyOf": [
        {
          "title": "Yaw/Pitch/Roll",
          "properties": {
            "pitch": {
              "type": "number",
              "title": "Pitch"
            },
            "yaw": {
              "type": "number",
              "title": "Yaw"
            },
            "roll": {
              "type": "number",
              "title": "Roll"
            }
          },
          "required": [ "pitch", "yaw", "roll" ]
        },
        {
          "title": "Quaternion",
          "properties": {
            "x": { "type": "number" },
            "y": { "type": "number" },
            "z": { "type": "number" },
            "w": { "type": "number" }
          },
          "required": [ "x", "y", "z", "w" ]
        }
      ]
    },
    "mat4": {
      "type": "array",
      "items": { "type": "number" },
      "minItems": 16,
      "maxItems": 16
    },
    "ivec2": {
      "type": "object",
      "properties": {
        "x": { "type": "integer" },
        "y": { "type": "integer" }
      },
      "required": [ "x", "y" ],
      "additionalProperties": false
    },
    "vec2": {
      "type": "object",
      "properties": {
        "x": { "type": "number" },
        "y": { "type": "number" }
      },
      "required": [ "x", "y" ],
      "additionalProperties": false
    },
    "vec3": {
      "type": "object",
      "properties": {
        "x": { "type": "number" },
        "y": { "type": "number" },
        "z": { "type": "number" }
      },
      "required": [ "x", "y", "z" ],
      "additionalProperties": false
    },
    "color": {
      "type": "object",
      "properties": {
        "r": {
          "type": "number",
          "minimum": 0.0,
          "maximum": 1.0
        },
        "g": {
          "type": "number",
          "minimum": 0.0,
          "maximum": 1.0
        },
        "b": {
          "type": "number",
          "minimum": 0.0,
          "maximum": 1.0
        },
        "a": {
          "type": "number",
          "minimum": 0.0,
          "maximum": 1.0
        }
      },
      "required": [ "r", "g", "b", "a" ]
    },

    "address": {
      "type": "string",
      "title": "Address",
      "description": "The IP address or the DNS name of the node. If the address is a DNS name, the name resolution is delegated to the operating system and might include additional network traffic to the DNS host. If the node ought to be the local machine either 127.0.0.x with x from 0 to 255, or localhost (which corresponds to 127.0.0.1) can be used"
    },
    "display": {
      "type": "object",
      "properties": {
        "swapinterval": {
          "type": "integer",
          "minimum": 0,
          "title": "Swap Interval",
          "description": "Determines the swap interval for the application. This determines the amount of V-Sync that should occur for the application. The two most common values for this are 0 for disabling V-Sync and 1 for regular V-Sync. The number provided determines the number of screen updates to wait before swapping the backbuffers and returning. For example on a 60Hz monitor, swapinterval=\"1\" would lead to a maximum of 60Hz frame rate, swapinterval=\"2\" would lead to a maximum of 30Hz frame rate. Using the same values for a 144Hz monitor would be a refresh rate of 144 and 72 respectively. The default value is 0, meaning that V-Sync is disabled"
        },
        "refreshrate": {
          "type": "integer",
          "minimum": 0,
          "title": "Refresh Rate",
          "description": "Determines the desired refresh rate for full-screen windows of this configuration. This value is disabled for windowed mode windows. The default value is the highest possible refresh rate"
        }
      },
      "title": "Display",
      "description": "Settings specific for the handling of display-related settings for the whole application"
    },
    "draw2d": {
      "type": "boolean",
      "title": "Draw 2D",
      "description": "Determines whether the draw2D callback should be called for viewports in this window. The default value is true"
    },
    "draw3d": {
      "type": "boolean",
      "title": "Draw 3D",
      "description": "Determines whether the draw callback should be called for viewports in this window. The default value is true"
    },
    "fullscreen": {
      "type": "boolean",
      "title": "Full Screen",
      "description": "Determines whether the window should be created in an exclusive fullscreen mode. The Size of this window will be used to set the screen resolution if this value is true. See also the monitor attribute to determine which monitor should be used as the target for the fullscreen window. The default value is false"
    },
    "id": {
      "type": "integer",
      "description": "The numerical identifier of this window. By default windows are given numerical value equal to their position in the node-1, meaning that the first window of a node will have the id 0, the second window id 1, etc. This value can be used to overwride this. It is not possible to give the same ID to two different windows"
    },
    "masteraddress": {
      "type": "string",
      "title": "Master Address",
      "description": "Contains the address of the node that acts as the server for this cluster. This means that one of the Node elements described in this configuration file has to have an address that corresponds to this masterAddress. This value can be either an IP address or a DNS name, which will be resolved at application startup"
    },
    "monitor": {
      "type": "integer",
      "title": "Monitor",
      "description": "Determines which monitor should be used for the exclusive fullscreen in case fullscreen is set to true. The list of monitors on the system are zero-based and range between 0 and the number of monitors - 1. For this attribute, the special value -1 can be used to denote that the primary monitor as defined by the operating system should be used, regardless of its index. The default value is -1"
    },
    "port": {
      "type": "integer",
      "minimum": 0,
      "title": "Port",
      "description": "The port at which this node is available at to the server. Since the server has to open bidirectional sockets to all of the client nodes, the port for each Node has to be mutually exclusive, meaning that no two Nodes can have the same port. Please note that operating systems have restricted behavior when trying to open ports lower than a fixed limt. For example, Unix does not allow non-elevated users to open ports < 1024. As a convention, SGCT usually uses ports staring at 20400, but this is an arbitrary convention without a specific reason"
    },
    "projectionquality": {
      "type": "string",
      "enum": [
        "low", "256", "medium", "512", "high", "1k", "1024", "1.5k", "1536", "2k", "2048",
        "4k", "4096", "8k", "8192", "16k", "16384", "32k", "32768", "64k", "65536"
      ],
      "title": "Quality",
      "description": "Determines the pixel resolution of the cube map faces that are reprojected to create the fisheye rendering. The higher resolution these cube map faces have, the better quality the resulting fisheye rendering, but at the expense of increased rendering times"
    },
    "tags": {
      "type": "array",
      "items": {
        "type": "string"
      },
      "title": "Tags",
      "description": "A comma-separated list of tags that are associated with this window. The tags themselves don't have any meaning inside of SGCT, but can be used by the application code as a filter. One common use-case is to tag one of the windows as a “GUI” window, to restrict input to only that window, for example. The default value for this attribute is an empty string"
    },
    "tracked": {
      "type": "boolean",
      "title": "Tracked",
      "description": "Determines whether the field-of-view frustum used for this viewport should be tracking changes to the window configuration. If this value is set to false, the field of view set in the beginning will stay unchanged"
    },
    "viewportpos": {
      "$ref": "#/$defs/vec2",
      "title": "Position",
      "description": "Specifies the position of the viewport inside its parent Window. The coordinates for x and y, which must both be specified in this node, are usually between 0 and 1, but are not restricted. Parts of the viewport that are outside this range would lie outside the bounds of the window and are clipped. Viewports are free to overlap and the viewports are rendered top to bottom into the window and can overwrite previous results"
    },
    "viewportsize": {
      "$ref": "#/$defs/vec2",
      "title": "Size",
      "description": "Specifies the size of this viewport inside its parent Window. The coordinate for x and y, which must both be specified in this node, are between 0 and 1, but are not restricted. Parts of the viewport that are outside this range would lie outside the bounds of the window and are clipped. Viewports are free to overlap and the viewports are rendered top to bottom into the window and can overwrite previous results"
    },
    "windowname": {
      "type": "string",
      "title": "Name",
      "description": "The name of the window. This is also used as the title of the window if window decorations are enabled. The default name for a window if this value is not specified is “SGCT Node: %i (%s)” with %i = the address of this node and %s either “server” or “client”, depending on whether the current node is the server or the client in the cluster"
    },
    "windowborder": {
      "type": "boolean",
      "title": "Border",
      "description": "Enables or disables the window decorations. Window decorations are the title bar that contains the name of the window and buttons to close, maximize or minimize a window. On some operating systems, the window decorations also include a border around a window and potentially shadow effects, all of which can be disabled with this attribute. The default is true"
    },
    "windowpos": {
      "$ref": "#/$defs/ivec2",
      "title": "Position",
      "description": "Sets the position of the window on the overall desktop space provided by the operating system. This node must have x and y floating point attributes that specify the x-y location of the window. Please note that these values also can be negative on some operating systems. On Windows, for example, the top left corner of the primary monitor is (0,0) in this coordinate system, but there can be additional monitors to the left or the top of the primary monitor, which would require negative numbers. The default values are x=0 and y=0"
    },
    "windowsize": {
      "$ref": "#/$defs/ivec2",
      "title": "Size",
      "description": "Sets the size of the window in pixels. This node must have x and y floating point attributes that determine that size of the window. The default values are x=640 and y=480"
    },

    "fovhorizontalvertical": {
      "type": "object",
      "properties": {
        "hfov": {
          "type": "number",
          "minimum": 0.0,
          "title": "Horizontal Field-of-view",
          "description": "The horizontal angle (in degrees) of the camera"
        },
        "vfov": {
          "type": "number",
          "minimum": 0.0,
          "title":" Vertical Field-of-view",
          "description": "The vertical angle (in degrees) of the camera"
        }
      },
      "required": [ "hfov", "vfov" ],
      "title": "hFov / vFov"
    },
    "fovleftrightvertical": {
      "type": "object",
      "properties": {
        "left": {
          "type": "number",
          "title": "Left",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the left border of the of the viewport. The left and right angles added together are the vertical field of view of the viewport"
        },
        "right": {
          "type": "number",
          "title": "Right",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the right border of the of the viewport. The left and right angles added together are the vertical field of view of the viewport"
        },
        "vfov": {
          "type": "number",
          "minimum": 0.0,
          "title":" Vertical Field-of-view",
          "description": "The vertical angle (in degrees) of the camera"
        }
      },
      "required": [ "left", "right", "vfov" ],
      "title": "left+right / vFov"
    },
    "fovdownuphorizontal": {
      "type": "object",
      "properties": {
        "hfov": {
          "type": "number",
          "minimum": 0.0,
          "title": "Horizontal Field-of-view",
          "description": "The horizontal angle (in degrees) of the camera"
        },
        "down": {
          "type": "number",
          "title": "Down",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the bottom border of the of the viewport. The down and up angles added together are the vertical field of view of the viewport"
        },
        "up": {
          "type": "number",
          "title": "Up",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the top border of the of the viewport. The down and up angles added together are the vertical field of view of the viewport"
        }
      },
      "required": [ "hfov", "down", "up" ],
      "title": "hFov / down+up"
    },
    "fovleftrightupdown": {
      "type": "object",
      "properties": {
        "left": {
          "type": "number",
          "title": "Left",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the left border of the of the viewport. The left and right angles added together are the vertical field of view of the viewport"
        },
        "right": {
          "type": "number",
          "title": "Right",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the right border of the of the viewport. The left and right angles added together are the vertical field of view of the viewport"
        },
        "down": {
          "type": "number",
          "title": "Down",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the bottom border of the of the viewport. The down and up angles added together are the vertical field of view of the viewport"
        },
        "up": {
          "type": "number",
          "title": "Up",
          "description": "The angle (in degrees) that is covered by the camera between the central point and the top border of the of the viewport. The down and up angles added together are the vertical field of view of the viewport"
        }
      },
      "required": [ "left", "right", "down", "up" ],
      "title": "left+right / down+up"
    },

    "planarprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "PlanarProjection",
          "default": "PlanarProjection",
          "readOnly": true
        },
        "fov": {
          "oneOf": [
            { "$ref": "#/$defs/fovhorizontalvertical" },
            { "$ref": "#/$defs/fovleftrightvertical" },
            { "$ref": "#/$defs/fovdownuphorizontal" },
            { "$ref": "#/$defs/fovleftrightupdown" }
          ],
          "title": "Camera Field-of-View",
          "description": "This element describes the field of view used the camera in this planar projection"
        },
        "distance": {
          "type": "number",
          "title": "Distance",
          "description": "The distance (in meters) at which the virtual render plane is placed. This value is only important when rendering this viewport using stereocopy as the distance and the Users eyeSeparation are used to compute the change in frustum between the left and the right eyes"
        },
        "orientation": {
          "$ref": "#/$defs/quat",
          "title": "Orientation",
          "description": "Describes a fixed orientation for the virtual image plane. This can be provided either as Euler angles or as a quaternion"
        },
        "offset": {
          "$ref": "#/$defs/vec3",
          "title": "Offset",
          "description": "A linear offset in meters that is added to the virtual image plane. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, meaning that no offset is applied to the image plane"
        }
      },
      "required": ["fov"],
      "description": "Describes a projection for the Viewport that is a flat projection described by simple frustum, which may be asymmetric"
    },

    "fisheyeprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "FisheyeProjection",
          "default": "FisheyeProjection",
          "readOnly": true
        },
        "fov": {
          "type": "number",
          "minimum": 0.0,
          "title": "Field-of-View",
          "description": "Describes the field of view that is covered by the fisheye projection in degrees. The resulting image will always be a circle, and this value determines how much of a field of view is covered by this circle. Please note specifically that this also includes field-of-view settings >180, in which a larger distortion is applied to the image. The default value is 180"
        },
        "quality": {
          "$ref": "#/$defs/projectionquality"
        },
        "interpolation": {
          "type": "string",
          "enum": [ "cubic", "linear" ],
          "title": "Interpolation",
          "description": "Determines the texture interpolation method that is used by SGCT when reprojecting the cube maps into the final fisheye rendering. The default value is “linear”"
        },
        "diameter": {
          "type": "number",
          "title": "Diameter",
          "description": "Sets the diameter in meters for the “sphere” that the fisheye is reprojected based on. This value is only used for stereoscopic rendering to compute the frustum offset using the Users eyeSeparation. The default value is 14.8"
        },
        "tilt": {
          "type": "number",
          "title": "Tilt",
          "description": "Determines the tilt of the “up vector” of the fisheye. With a tilt of 0, the center of the fisheye image is the apex of the half-sphere that is used to reproject the cube map. A tilted fisheye rendering is useful when projecting on a tilted planetarium dome. A value of 90, for example, would result in the forward facing direction being at the center of the fisheye circular image. The default value is 0"
        },
        "crop": {
          "type": "object",
          "properties": {
            "left": {
              "type": "number",
              "title": "Left",
              "description": "The ratio of the image that is cropped from the left. If the value is 0, the image is not cropped at all from this side, if it is 1, the entire image is cropped. However, this cropping value must not be larger than the 1 - right cropping value as these value might not overlap. The default value is 0"
            },
            "right": {
              "type": "number",
              "title": "Right",
              "description": "The ratio of the image that is cropped from the right. If the value is 0, the image is not cropped at all from this side, if it is 1, the entire image is cropped. However, this cropping value must not be larger than the 1 - left cropping value as these value might not overlap. The default value is 0"
            },
            "bottom": {
              "type": "number",
              "title": "Bottom",
              "description": "The ratio of the image that is cropped from the bottom. If the value is 0, the image is not cropped at all from this side, if it is 1, the entire image is cropped. However, this cropping value must not be larger than the 1 - top cropping value as these value might not overlap. The default value is 0"
            },
            "top": {
              "type": "number",
              "title": "Top",
              "description": "The ratio of the image that is cropped from the top. If the value is 0, the image is not cropped at all from this side, if it is 1, the entire image is cropped. However, this cropping value must not be larger than the 1 - bottom cropping value as these value might not overlap. The default value is 0"
            }
          },
          "title": "Crop",
       	  "required": ["left", "right", "bottom", "top"],
          "description": "This node can be used to crop the fisheye after the post processing has been performed. This might be useful for domes running a single projector with a fisheye lens. Normally a projector has a 16:9, 16:10, or 4:3 aspect ratio, but the fiehye output has a 1:1 aspect ratio. This circle can be squared by cropping the 1:1 aspect ratio fisheye image down to the aspect ratio of the projector that is used. By default, no cropping is applied to the image, leaving it in a 1:1 aspect ratio"
        },
        "keepaspectratio": {
          "type": "boolean",
          "title": "Keep Aspect Ratio",
          "description": "Determines whether the application should try to maintain the original aspect ratio when resizing the window or whether the field of view should be recalculated based on the window's new aspect ratio. The default value is true"
        },
        "offset": {
          "$ref": "#/$defs/vec3",
          "title": "Offset",
          "description": "A linear offset in meters that is added to the virtual planes used to project the fisheye rendering. This can be used for off-axis projections. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, meaning that no offset is applied to the center of the fisheye sphere"
        },
        "background": {
          "$ref": "#/$defs/color",
          "title": "Background",
          "description": "This value determines the color that is used for the parts of the image that are not covered by the spherical fisheye image. The alpha component of this color has to be provided even if the final render target does not contain an alpha channel, in which case the alpha value is ignored. All attributes r, g, b, and a must be defined and be between 0 and 1. The default color is a dark gray (0.3, 0.3, 0.3, 1.0)"
        }
      },
      "required": [ "type" ],
      "description": "Describes a fisheye projection that is used to render into its parent Viewport. By default, a fisheye rendering is covering 180 degrees field of view and has a 1:1 aspect ratio, though these parameters can be changed with the attributes provided in this node. This projection type counts as a non-linear projection, which requires 4-6 render passes of the application, meaning that the application might render slower when using these kind of projections than a flat projection. In either case, the application does not need to be aware of the projection as this abstract is handled internally and the applications draw method is only called multiple times per frame with different projection methods that are used to create the full fisheye projection"
    },

    "sphericalmirrorprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "SphericalMirrorProjection",
          "default": "SphericalMirrorProjection",
          "readOnly": true
        },
        "quality": {
          "$ref": "#/$defs/projectionquality"
        },
        "tilt": {
          "type": "number",
          "title": "Tilt",
          "description": "Determines the tilt of the “up vector” of the spherical mirror projection. With a tilt of 0, the center of the spherical mirror image is the apex of the half-sphere that is used to reproject the cube map. The default value is 0"
        },
        "background": {
          "$ref": "#/$defs/color",
          "title": "Background",
          "description": "This value determines the color that is used for the parts of the image that are not covered by the spherical mirror image. The alpha component of this color has to be provided even if the final render target does not contain an alpha channel, in which case the alpha value is ignored. All attributes r, g, b, and a must be defined and be between 0 and 1. The default color is a dark gray (0.3, 0.3, 0.3, 1.0)"
        },
        "geometry": {
          "type": "object",
          "properties": {
            "bottom": {
              "type": "string",
              "title": "Bottom",
              "description": "The path to the warping mesh that is loaded for the bottom part of the spherical mirror projection"
            },
            "left": {
              "type": "string",
              "title": "Left",
              "description": "The path to the warping mesh that is loaded for the bottom part of the spherical mirror projection"
            },
            "right": {
              "type": "string",
              "title": "Right",
              "description": "The path to the warping mesh that is loaded for the bottom part of the spherical mirror projection"
            },
            "top": {
              "type": "string",
              "title": "Top",
              "description": "The path to the warping mesh that is loaded for the bottom part of the spherical mirror projection"
            }
          },
          "title": "Geometry",
          "description": "Describes the warping meshes used for the spherical mirror projection. All four warping meshes have to be present"
        }
      },
      "required": [ "type", "geometry" ],
      "description": "Used to create a projection used for Paul Bourke's spherical mirror setup (see here), which makes it possible to use an off-the-shelf projector to create a planetarium-like environment by bouncing the image of a shiny metal mirror. Please note that this is not the only way to produce these kind of images. Depending on your setup and availability of warping meshes, it might suffice to use the FisheyeProjection node type instead and add a single mesh to the parent Viewport instead. The config folder in SGCT contains an example of this using a default 16x9 warping mesh. This projection type specifically deals with the case where you have four different meshes, one for the bottom, top, left, and right parts of the distorted image"
    },

    "spoutoutputprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "SpoutOutputProjection",
          "default": "SpoutOutputProjection",
          "readOnly": true
        },
        "quality": {
          "$ref": "#/$defs/projectionquality"
        },
        "mapping": {
          "type": "string",
          "enum": [ "fisheye", "equirectangular", "cubemap" ],
          "title": "Mapping",
          "description": "Determines the type of sharing that occurs with this projection and thus how many and which texture is shared via Spout. For the “fisheye” and “equirectangular”, only the single, final reprojected image is shared, for the “cubemap” method, all selected cubemaps will be provided through the Spout interface. The default value is “cubemap”"
        },
        "mappingspoutname": {
          "type": "string",
          "title": "Mapping Spout Name",
          "description": "Sets the name of the texture if the mapping type is 'fisheye' or 'equirectangular'. If the mapping is 'cubemap', this value is ignored"
        },
        "background": {
          "$ref": "#/$defs/color",
          "title": "Background",
          "description": "This value determines the color that is used for the parts of the image that are not covered. The alpha component of this color has to be provided even if the final render target does not contain an alpha channel. All attributes r, g, b, and a must be defined and be between 0 and 1. The default color is a dark gray (0.3, 0.3, 0.3, 1.0)"
        },
        "channels": {
          "type": "object",
          "properties": {
            "right": {
              "type": "boolean",
              "title": "Right",
              "description": "If true, the cubemap face to the right is enabled"
            },
            "zleft": {
              "type": "boolean",
              "title": "Front",
              "description": "If true, the cubemap face to the front is enabled"
            },
            "bottom": {
              "type": "boolean",
              "title": "Bottom",
              "description": "If true, the cubemap face to the bottom is enabled"
            },
            "top": {
              "type": "boolean",
              "title": "Top",
              "description": "If true, the cubemap face to the top is enabled"
            },
            "left": {
              "type": "boolean",
              "title": "Left",
              "description": "If true, the cubemap face to the left is enabled"
            },
            "zright": {
              "type": "boolean",
              "title": "Back",
              "description": "If true, the cubemap face to the back is enabled"
            }
          },
          "required": [ "right", "zleft", "bottom", "top", "left", "zright" ],
          "title": "Channels",
          "description": "Determines for the “cubemap” mapping type, which cubemap faces should be rendered and shared via spout. "
        },
        "orientation": {
          "type": "object",
          "properties": {
            "pitch": { "type": "number" },
            "yaw": { "type": "number" },
            "roll": { "type": "number" }
          },
          "required": [ "pitch", "yaw", "roll" ]
        }
      },
      "required": [ "type", "mappingspoutname" ],
      "additionalProperties": false,
      "description": "Provides the ability to share individual cube map faces or a fully reprojected image using the Spout library. This library only supports the Windows operating system, so this projection will only work on Windows machines. Spout's functionality is the abilty to shared textures between different applications on the same machine, making it possible to render images using SGCT and making them available to other real-time applications on the same machine for further processing. Spout uses a textual name for accessing which texture should be used for sharing. The SpoutOutputProjection has three different output types, outputting each cube map face, sharing a fisheye image, or sharing an equirectangular projection, as determined by the mapping attribute"
    },

    "spoutflatprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "SpoutFlatProjection",
          "default": "SpoutFlatProjection",
          "readOnly": true
        },
        "width": {
          "type": "integer",
          "title": "Width",
          "description": "The width of the individual spout faces being shared in pixels"
        },
        "height": {
          "type": "integer",
          "title": "Height",
          "description": "The height of the individual spout faces being shared in pixels"
        },
        "mappingSpoutName": {
          "type": "string",
          "title": "Mapping Spout Name",
          "description": "Sets the base name of the texture"
        },
        "background": {
          "$ref": "#/$defs/color",
          "title": "Background",
          "description": "This value determines the color that is used for the parts of the image that are not covered. The alpha component of this color has to be provided even if the final render target does not contain an alpha channel. All attributes r, g, b, and a must be defined and be between 0 and 1. The default color is a dark gray (0.3, 0.3, 0.3, 1.0)"
        },
        "planarprojection": {
          "$ref": "#/$def/planarprojection",
          "title": "Planarprojection",
          "description": "The planar projection that is used for the forward facing standard rendering"
        }
      },
      "required": [ "type" ],
      "additionalProperties": false,
      "description": "Provides the ability to share individual cube map faces or a fully reprojected image using the Spout library and also share a planar projection using a seventh spout name simultaneously. This library only supports the Windows operating system, so this projection will only work on Windows machines. Spout's functionality is the abilty to shared textures between different applications on the same machine, making it possible to render images using SGCT and making them available to other real-time applications on the same machine for further processing. Spout uses a textual name for accessing which texture should be used for sharing. The SpoutOutputProjection has three different output types, outputting each cube map face, sharing a fisheye image, or sharing an equirectangular projection, as determined by the mapping attribute"
    },

    "cylindricalprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "CylindricalProjection",
          "default": "CylindricalProjection",
          "readOnly": true
        },
        "quality": {
          "$ref": "#/$defs/projectionquality"
        },
        "rotation": {
          "type": "number",
          "title": "Rotation",
          "description": "Provides a rotation angle (in radians) why which the cylindrical projection is offset into the resulting image"
        },
        "heightoffset": {
          "type": "number",
          "title": "Height Offset",
          "description": "Offsets the height from which the cylindrical projection is generated. This is, in general, only necessary if the user position is offset and you want to counter that offset to continue producing a “standard” cylindrical projection"
        },
        "radius": {
          "type": "number",
          "title": "Radius",
          "description": "Sets the radius of the sphere, which is only used in the cases when stereoscopic rendering is used"
        }
      },
      "required": [ "type" ],
      "additionalProperties": false,
      "description": "This projection method renders the scene into a view that can be mapped on the inside or outside of a cylinder. This projection method is support by some live media curation tools. The forward-facing direction will be at the left border of the image unless changed via the rotation option"
    },

    "equirectangularprojection": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "EquirectangularProjection",
          "default": "EquirectangularProjection",
          "readOnly": true
        },
        "quality": {
          "$ref": "#/$defs/projectionquality"
        }
      },
      "required": [ "type" ],
      "additionalProperties": false,
      "description": "This projection method renders an equirectangular projection (also called lat-long projection or equidistant cylindrical projection) is a default projection method used for spherical objects or maps"
    },

    "projectionplane": {
      "type": "object",
      "properties": {
        "type": {
          "type": "string",
          "const": "ProjectionPlane",
          "default": "ProjectionPlane",
          "readOnly": true
        },
        "lowerleft": {
          "$ref": "#/$defs/vec3",
          "title": "Lower Left Corner",
          "description": "The position of the lower left corner of the projection plane"
        },
        "upperleft": {
          "$ref": "#/$defs/vec3",
          "title": "Upper Left Corner",
          "description": "The position of the upper left corner of the projection plane"
        },
        "upperright": {
          "$ref": "#/$defs/vec3",
          "title": "Upper Right Corner",
          "description": "The position of the upper right corner of the projection plane"
        }
      },
      "required": [ "type", "lowerleft", "upperleft", "upperright" ],
      "additionalProperties": false,
      "description": "This projection method is based on providing three corner points that are used to construct a virtual image plane. The units of the positions are all relative to the user's position so are ultimately arbitrary"
    },

    "projection": {
      "oneOf": [
        {
          "$ref": "#/$defs/planarprojection",
          "title": "Planar Projection"
        },
        {
          "$ref": "#/$defs/fisheyeprojection",
          "title": "Fisheye Projection"
        },
        {
          "$ref": "#/$defs/sphericalmirrorprojection",
          "title": "Spherical Mirror Projection"
        },
        {
          "$ref": "#/$defs/spoutoutputprojection",
          "title": "Spout Output Projection"
        },
        {
          "$ref": "#/$defs/cylindricalprojection",
          "title": "Cylindrical Projection"
        },
        {
          "$ref": "#/$defs/equirectangularprojection",
          "title": "Equirectangular Projection"
        },
        {
          "$ref": "#/$defs/projectionplane",
          "title": "Projection Plane"
        }
      ],
      "title": "Projection"
    },

    "viewport": {
      "type": "object",
      "properties": {
        "user": {
          "type": "string",
          "title": "User",
          "description": "The name of the User that this viewport should be linked to. If a viewport is linked to a user that has a sensor, the positions of the sensor will be automatically reflected in the user position that is used to render this viewport. The default is that no user is linked with this viewport"
        },
        "overlay": {
          "type": "string",
          "title": "Overlay",
          "description": "This attribute is a path to an overlay texture that is rendered on top of the viewport after the applications rendering is finished. This can be used to add logos or other static assets on top of an application. The default is that no overlay is rendered"
        },
        "blendmask": {
          "type": "string",
          "title": "Blend Mask",
          "description": "This value is a path to a texture that is used as a mask to remove parts of the rendered image. The image that is provided in this should be a binary black-white image which is applied by SGCT after the application loading is finished. All parts where the mask image is black will be removed. The default is that no mask is applied"
        },
        "blacklevelmask": {
          "type": "string",
          "title": "Blacklevel Mask",
          "description": "The file referenced in this attribute is used as a postprocessing step for this viewport. The image should be a grayscale image, where each pixel will be multiplied with the resulting image from the application in order to perform a black level adaptation. If a pixel is completely white, the resulting pixel is the same as the applications output, if a pixel is black, the resulting pixel will be back, if it is 50% grey, the resolution pixel will be half brightness. The default is that no black level mask is applied"
        },
        "mesh": {
          "type": "string",
          "title": "Mesh",
          "description": "Determines a warping mesh file that is used to warp the resulting image. The application's rendering will always be rendered into a rectangular framebuffer, which is then mapped as a texture to the geometry provided by this file. This makes it possible to create non-linear or curved output geometries from a regular projection by providing the proper geometry of the surface that you want to project on. The reader for the warping mesh is determined by the file extension of the file that is provided in this attribute. The default is that no warping mesh is applied. Supported geometry mesh formats: 1. SCISS Mesh (sgc extension). A mesh format that was introduced by SCISS for the Uniview software. SCISS created two versions for this file format, one for 2D warping meshes and a second for 3D cubemap lookups. SGCT only supports the first version of the file format, however. 2. Scalable Mesh (ol extension) A mesh format created by scalable. 3. Dome Projection (csv extension), 4. Paul Bourke Mesh (data extension) A file format created by Paul Bourke, his webpage also contains more information abuot the individual steps of the warping. 5. Waveform OBJ (obj extension) The well known textual mesh format. 6. MPCDI (mpcdi extension) A subset of the VESA standard. 7. SimCAD (simcad extension)"
        },
        "tracked": {
          "$ref": "#/$defs/tracked"
        },
        "eye": {
          "type": "string",
          "enum": [ "center", "left", "right" ],
          "title": "Eye",
          "description": "Forces this viewport to be rendered with a specific eye, using the corresponding Users eye separation to compute the correct frustum. If this value is not set, the viewport will be rendered according to the parent Window's stereo attribute"
        },
        "pos": {
          "$ref": "#/$defs/viewportpos"
        },
        "size": {
          "$ref": "#/$defs/viewportsize"
        },
        "projection": {
          "$ref": "#/$defs/projection"
        }
      },
      "description": "Describes a single viewport inside a Window. Every window can contain an arbitrary number of viewports that are all rendered independently. The viewports are positioned inside the window using a normalized coordinate system"
    },

    "window": {
      "type": "object",
      "properties": {
        "id": {
            "$ref": "#/$defs/id"
        },
        "name": {
          "$ref": "#/$defs/windowname"
        },
        "tags": {
          "$ref": "#/$defs/tags"
        },
        "bufferbitdepth": {
          "type": "string",
          "enum": [ "8", "16", "16f", "32f", "16i", "32i", "16ui", "32ui" ],
          "title": "Buffer Bit Depth",
          "description": "Sets the bit depth and format of the color texture that is used as the render backend for this entire window. The parameters passed into this attribute are converted to the following OpenGL parameters (internal color format and data type) to the texture creation: 8: GL_RGBA8, GL_UNSIGNED_BYTE, 16: GL_RGBA16, GL_UNSIGNED_SHORT, 16f: GL_RGBA16F, GL_HALF_FLOAT, 32f: GL_RGBA32F, GL_FLOAT, 16i: GL_RGBA16I, GL_SHORT, 32i: GL_RGBA32I, GL_INT, 16ui: GL_RGBA16UI, GL_UNSIGNED_SHORT, 32ui: GL_RGBA32UI, GL_UNSIGNED_INT. The default value for this attribute is 8"
        },
        "fullscreen": {
          "$ref": "#/$defs/fullscreen"
        },
        "autoiconify": {
          "type": "boolean",
          "title": "Auto-Iconify",
          "description": "Determines whether an exclusive fullscreen window will be automatically iconified if it loses focus. This value will be ignored if the fullscreen value is not set or if it is false. The default value for this setting is false"
        },
        "hidemousecursor": {
          "type": "boolean",
          "title": "Hide Mouse Cursor",
          "description": "If this value is set to true, the mouse cursor will never be visible above this window. The default value for this setting is false"
        },
        "floating": {
          "type": "boolean",
          "title": "Floating",
          "description": "Indicates whether the window is floating, meaning that it is rendered by the operating system always on top of other windows. The default value is false"
        },
        "alwaysrender": {
          "type": "boolean",
          "title": "Always Render",
          "description": "Determines whether the content of the window should continue to render even if the window is not visible on screen. Normally, the operating system will not invalidate a window when it is hidden (see isHidden attribute) and this attribute can be used to overwrite that behavior. The default behavior is false"
        },
        "hidden": {
          "type": "boolean",
          "title": "Hidden",
          "description": "Determines whether this window should be visible on screen or used as an offscreen rendering target. If a window is hidden, you should also set alwaysRender to true, or otherwise the rendering might not occur as expected. The default for this attribute is false"
        },
        "doublebuffered": {
          "type": "boolean",
          "title": "Double Buffered",
          "description": "Sets the buffering to single buffering (if false) or double buffering or quad buffering for stereo (if true). The default is true"
        },
        "msaa": {
          "type": "integer",
          "minimum": 0,
          "title": "MSAA",
          "description": "Determines whether multisample antialiasing is used for the window and how many subsamples should be used for the antialiasing. If the value is set to 0, MSAA is disabled. MSAA operates by rendering the scene at a higher resolution using multiple samples per pixel and combining these samples to reduce aliasing. It produces good-looking results, but it increases the rendering time for the scene. The maximum number of samples depends on the GPU that is used to start the application, but is usually around 32. The default is 0, disabling MSAA"
        },
        "alpha": {
          "type": "boolean",
          "title": "Alpha",
          "description": "Determines whether screenshots created from this window should include an alpha channel or not. If this value is false, the resulting image type is RGB, otherwise RGBA. The default is false"
        },
        "fxaa": {
          "type": "boolean",
          "title": "FXAA",
          "description": "Determines whether fast approximate antialiasing is used for the contents of this window. This antialiasing is a postprocessing that does not significantly increase rendering time, but the results are not as good as msaa. The default is false"
        },
        "border": {
          "$ref": "#/$defs/windowborder"
        },
        "resizable": {
          "type": "boolean",
          "title": "Border",
          "description": "Enables or disables the ability of the user to change the size of the window. The default is true"
        },
        "mirror": {
          "type": "boolean",
          "title": "Mirror",
          "description": "Flips the resulting image vertically"
        },
        "draw2d": {
          "$ref": "#/$defs/draw2d"
        },
        "draw3d": {
          "$ref": "#/$defs/draw3d"
        },
        "blitwindowid": {
          "type": "integer",
          "title": "Blit Window ID",
          "minimum": -1,
          "description": "If this value is specified, the 3D contents of a different window are blitted (=copied) into this window before calling its own rendering. A common use-case for this are GUI windows that want to show the 3D rendering but not take the performance impact of rendering an expensive scene twice. Instead of rendering the 3D scene, a GUI window would set draw3D to false and this attribute to the id of the main window, meaning that the contents of that other window are copied and then the 2D UI will be rendered on top of the blitted content. Unless specified otherwise, a window’s id is its position in the XML file inside a node, starting at 0. So the first window of a node will have the id 0, the second 1, etc. The default value is -1 which means taht not blitting is performed"
        },
        "monitor": {
          "$ref": "#/$defs/monitor"
        },
        "mpcdi": {
          "type": "string",
          "title": "MPCDI",
          "description": "If this value is set to a path that contains an MPCDI file that describes camera parameters and warping and blending masks, these values are used to initialize the contents of this window instead of providing explicit viewport information. If this value is used, there cannot be any Viewports defined in this window as as the mpcdi file takes care of this. The default value is that no MPCDI is used"
        },
        "stereo": {
          "type": "string",
          "enum": [
            "none", "no_stereo",
            "active", "quadbuffer",
            "checkerboard",
            "checkerboard_inverted",
            "anaglyph_red_cyan",
            "anaglyph_amber_blue",
            "anaglyph_wimmer",
            "vertical_interlaced",
            "vertical_interlaced_inverted",
            "test", "dummy",
            "side_by_side",
            "side_by_side_inverted",
            "top_bottom",
            "top_bottom_inverted"
          ],
          "title": "Stereo",
          "description": "Determines whether the contents of this window should be rendered stereoscopically and which stereoscopic rendering method should be used. The only allowed attribute for this node is the type, which determines the type of stereo rendering. It has to be one of: 1. none: No stereo rendering is performed. This is the same as if this entire node was not specified. 2. active: Using active stereo using quad buffering. This is only a valid option for systems that support quad buffering. 3. checkerboard: Using a checkerboard pattern for stereoscopy in which left and right eyes are rendered on interleaved checkerboard patterns. 4. checkerboard_inverted: Using the same pattern as checkerboard, but with the left and right eyes inverted. 5. anaglyph_red_cyan: Applying color filters to the rendering for the left and right eyes such that red-cyan anaglyph glasses can be used to view the stereo content. 6. anaglyph_amber_blue: Applying color filters to the rendering for the left and right eyes such that amber-blue anaglyph glasses can be used to view the stereo content. 7. anaglyph_wimmer: ¯|(ツ)/¯. 8. vertical_interlaced: A stereo format in which the left and right eye images are interlaced vertically, meaning that each row of the final image is either left or right, switching each row. 9. vertical_interlaced_inverted: The same as vertical_interlaced, but with the left and right eye flipped. 10. dummy: A dummy stereo mode to test streoscopic rendering without needing extra equipment. In this stereo mode, the left and the right eye images are rendered on top of each other without any other processing. This option is available to verify that stereo rendering is working for a specific application. 11. side_by_side: The resolution of the window is split into a left half and a right half, with each eye being rendered into its half. This is a common stereo format for 3D TVs. 12. side_by_side_inverted: The same as side_by_side, but the left and right images are flipped. 13. top_bottom: The same as side_by_side, but instead of separating the window horizontally, the window is split vertically, with the left eye being rendered in the top half of the window and the right image being rendered in the bottom half. 14. top_bottom_inverted: The same as top_bottom_inverted, but with the left and right eyes flipped. The default value is none"
        },
        "pos": {
          "$ref": "#/$defs/windowpos"
        },
        "size": {
          "$ref": "#/$defs/windowsize"
        },
        "res": {
          "$ref": "#/$defs/ivec2",
          "title": "Resolution",
          "description": "Sets the size of the internal framebuffer that is used to render the contents of the window. In a lot of cases, this resolution is the same resolution as the size of the window, but it is a useful tool when creating images that are larger than a window would be support on an operating system. Some operating systems restrict windows to be no larger than what can fit on a specific monitor. This node must have x and y floating point attributes that determine that size of the window. By default the resolution of the framebuffer is equal to the size of the window"
        },
        "viewports": {
          "type": "array",
          "items": { "$ref": "#/$defs/viewport" },
          "title": "Viewports"
        }
      },
      "required": ["size"]
    },

    "node": {
      "type": "object",
      "properties": {
        "address": {
          "$ref": "#/$defs/address"
        },
        "port": {
          "$ref": "#/$defs/port"
        },
        "datatransferport": {
          "type": "integer",
          "minimum": 0,
          "title": "Data Transfer Port",
          "description": "If this value is set, a socket connection is opened at the provided port that can be used by the application to transmit data between the server and the clients using the dataTransfer* callbacks and transferData function of the NetworkManager. If no value is specified this function will not work and the callbacks will never be called. Please note that operating systems have restricted behavior when trying to open ports lower than a fixed limt. For example, Unix does not allow non-elevated users to open ports < 1024"
        },
        "swaplock": {
          "type": "boolean",
          "title": "Swap Lock",
          "description": "Determines whether this node should be part of an Nvidia swap group and should use the swap barrier. Please note that this feature only works on Windows and requires Nvidia Quadro cards + G-Sync synchronization cards. The default value is false"
        },
        "windows": {
          "type": "array",
          "items": { "$ref": "#/$defs/window" },
          "title": "Windows",
          "description": "Specifies a single window that is used to render content into. There can be an arbitrary(-ish) number of windows for each node and they all will be created and initialized at start time. Each window has at least one Viewport that specifies exactly where in the window the rendering occurs with which parameters"
        }
      },
      "required": [ "address", "port" ],
      "description": "Defines a single computing node that is contained in the described cluster. In general this corresponds to a single computer, but it is also possible to create multiple nodes on a local machine by using the 127.0.0.x IP address with x from 0 to 255. It is not possible to create multiple nodes on the same remote computer, however"
    },
    
    "user": {
      "type": "object",
      "properties": {
        "name": {
          "type": "string",
          "title": "Name",
          "description": "Specifies the name of this user. Each user needs to have a unique name, but there also has to be exactly one user present that has an empty name (or without a name attribute) which is used as the default user"
        },
        "eyeseparation": {
          "type": "number",
          "minimum": 0.0,
          "title": "Eye Separation",
          "description": "Determines the eye separation used for stereoscopic viewports. If no viewports in the configuration are using stereo, this setting is ignored"
        },
        "pos": {
          "$ref": "#/$defs/vec3",
          "title": "Position",
          "description": "A linear offset of the user position. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, meaning that no linear offset is applied to the user's position"
        },
        "matrix": {
          "$ref": "#/$defs/mat4",
          "title": "Matrix",
          "description": "A generic transformation matrix that is applied to the orientation of this user. This value will overwrite the value specified in Orientation"
        },
        "orientation": {
          "$ref": "#/$defs/quat",
          "title": "Orientation",
          "description": "Describes a fixed orientation for the viewing direction of this user. This can be provided either as Euler angles or as a quaternion"
        },
        "tracking": {
          "type": "array",
          "items": { 
            "type": "object",
            "properties": {
              "tracker": {
                "type": "string",
                "title": "Tracker",
                "description": "The name of the tracker group that this user should be linked with. This name must be a name of a Tracker that is specified in this configuration"
              },
              "device": {
                "type": "string",
                "title": "Device",
                "description": "The name of the device in the tracker group that should be used to control the tracking for this user. The specified device has to be a Device that was specified in the tracker"
              }
            },
            "required": [ "tracker", "device" ]
          },
          "title": "Tracking",
          "description": "Provides information about whether this user should be tracked using a VRPN-based tracker. This child node contains two attributes with information about the tracker that this user should be associated with"
        }
      },
      "description": "Specifies a user position and parameters. In most cases, only a single unnamed user is necessary. However, in more general cases, it is possible to assign Users to specific Viewports to provide a more fine-grained control over the rendering that occurs in that viewport"
    },

    "scene": {
      "type": "object",
      "properties": {
        "offset": {
          "$ref": "#/$defs/vec3",
          "title": "Offset",
          "description": "A linear offset of the scene center. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, which means that no offset is applied to the center of the scene"
        },
        "orientation": {
          "$ref": "#/$defs/quat",
          "title": "Orientation",
          "description": "Describes a fixed orientation of the global scene"
        },
        "scale": {
          "type": "number",
          "title": "Scale",
          "description": "A scaling factor for the entire scene. The default value is 1.0"
        }
      },
      "description": "Determines an overall orientation of the scene. It consists of an Offset, an Orientation, and Scale; all of which is included in the projection matrix that is passed to the rendering function callback of the specific application. This node can be used to customize the rendering for a specific rendering window. A common use-case in planetariums, for example, is to account for a tilt in the display system by providing an Orientation with the same pitch as the planetarium surface. This makes it possible to reuse the same application between the planetarium dome and fixed setups without the need for special care"
    },

    "settings": {
      "type": "object",
      "properties": {
        "depthbuffertexture": {
          "type": "boolean",
          "title": "Use Depth Buffer Texture",
          "description": "If this value is set to true and a non-linear projection method if provided in a window, SGCT will also provide a buffer containing the reprojected depth values of the non-linear projection. This value defaults to false"
        },
        "normaltexture": {
          "type": "boolean",
          "title": "Use Normal Texture",
          "description": "If this value is set to true and a non-linear projection method if provided in a window, SGCT will also provide a buffer containing the reprojected normals values of the non-linear projection. This value defaults to false"
        },
        "positiontexture": {
          "type": "boolean",
          "title": "Use Position Texture",
          "description": "If this value is set to true and a non-linear projection method if provided in a window, SGCT will also provide a buffer containing the reprojected positions of the non-linear projection. This value defaults to false"
        },
        "precision": {
          "type": "integer",
          "enum": [ 16, 32 ],
          "title": "Precision",
          "description": "Determines the floating point precision for the normal and position textures if they are enabled. Setting this value if NormalTexture and PositionTexture are disabled does not have any effect. This value defaults to 32"
        },
        "display": {
          "$ref": "#/$defs/display"
        }
      },
      "description": "Controls global settings that affect the overall behavior of the SGCT library that are not limited just to a single window"
    },

    "capture": {
      "type": "object",
      "properties": {
        "path": {
          "type": "string",
          "title": "Path",
          "description": "Sets the path used when creating screenshots. The default value is to use the current working directory"
        },
        "format": {
          "type": "string",
          "enum": [ "png", "PNG", "tga", "TGA", "jpg", "JPG" ],
          "title": "Format",
          "description": "Sets the screenshot format that should be used for the screenshots taken of the application. The default value is PNG"
        },
        "range-begin": {
          "type": "integer",
          "title": "Range (begin)",
          "description": "The index of the first screenshot that will actually be rendered. If this value is set, all previous screenshots will be ignored, but the counter will be increased either way. If this value is set, the range-end value also needs to be set. A value of -1 will mean the same thing as 0, saying the everything from the first frame will be captured, which is also the default"
        },
        "range-end": {
          "type": "integer",
          "title": "Range (end)",
          "description": "The index of the last screenshot that will not be rendered anymore. If this value is set, all screenshots starting with this index will be ignored. If this value is set, the range-begin value also needs to be set. A value of -1 will mean that all remaining screenshots will be captured, which is the default"
        }
      },
      "description": "Contains information relevant to capturing screenshots from an SGCT application"
    },

    "device": {
      "type": "object",
      "properties": {
        "name": {
          "type": "string",
          "title": "Name",
          "description": "Specifies the name of the device so that it can be referenced by a User or can be accessed programmatically by the application"
        },
        "sensors": {
          "type": "array",
          "items": {
            "type": "object",
            "properties": {
              "vrpnaddress": {
                "type": "string",
                "title": "VRPN Address",
                "description": "The VRPN address of this sensor"
              },
              "id": {
                "type": "integer",
                "description": "The sensor id for this device. This information is not used by SGCT directly but can be used by the application to distinguish different sensors if multiple sensors are specified in the configuration file"
              }
            },
            "required": [ "vrpnaddress", "id" ],
            "description": "Represents a tracked sensor that provides orientation and position information to the application. The sensors can be accessed through the TrackingManager class and its related classes"
          }
        },
        "buttons": {
          "type": "array",
          "items": {
            "type": "object",
            "properties": {
              "vrpnaddress": {
                "type": "string",
                "title": "VRPN Address",
                "description": "The VRPN address of this button group"
              },
              "count": {
                "type": "integer",
                "title": "Count",
                "description": "The number of buttons that are advertised and received through the Device"
              }
            },
            "required": [ "vrpnaddress", "count" ],
            "description": "Represents a group of toggle buttons that can be triggered through VRPN. The buttons can be accessed through the TrackingManager class and its related classes"
          }
        },
        "axes": {
          "type": "array",
          "items": {
            "type": "object",
            "properties": {
              "vrpnaddress": {
                "type": "string",
                "title": "VRPN Address",
                "description": "The VRPN address of this group of axes"
              },
              "count": {
                "type": "integer",
                "title": "Count",
                "description": "The number of axes that are advertised by this VRPN device"
              }
            },
            "required": [ "vrpnaddress", "count" ],
            "description": "Represents a number of independent 1D axes that are updated through VRPN. The axes can be accessed through the TrackingManager class and its related classes"
          }
        },
        "offset": {
          "$ref": "#/$defs/vec3",
          "title": "Offset",
          "description": "A linear offset that is added to the entire device. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, leading to no offset being applied"
        },
        "transformation": {
          "$ref": "#/$defs/mat4",
          "title": "Transformation",
          "description": "A generic transformation matrix that is applied to this device. This value will overwrite the value specified in Orientation. The attributes used for the matrix are named x0, y0, z0, w0, x1, y1, z1, w2, x2, y2, z2, w2, x3, y3, z3, w3 and are used in this order to initialize the matrix in a column-major order. All 16 of these values have to be present in this attribute and have to be floating point values"
        }
      },
      "required": [ "name" ],
      "description": "Specifies a single tracking device that belongs to a specific tracker group"
    },

    "tracker": {
      "type": "object",
      "properties": {
        "name": {
          "type": "string",
          "title": "Name",
          "description": "The name of the tracker group"
        },
        "devices": {
          "type": "array",
          "items": { "$ref": "#/$defs/device" },
          "title": "Devices"
        },
        "offset": {
          "$ref": "#/$defs/vec3",
          "title": "Offset",
          "description": "A linear offset of the class of trackers. Must define three float attributes x, y, and z. The default values are x=0, y=0, z=0, which means that no linear offset is applied to the tracker"
        },
        "orientation": {
          "$ref": "#/$defs/quat",
          "title": "Orientation"
        },
        "matrix": {
          "$ref": "#/$defs/mat4",
          "title": "Matrix",
          "description": "A generic transformation matrix that is applied to all trackers in this group. This value will overwrite the value specified in Orientation. The attributes used for the matrix are named x0, y0, z0, w0, x1, y1, z1, w2, x2, y2, z2, w2, x3, y3, z3, w3 and are used in this order to initialize the matrix in a column-major order. All 16 of these values have to be present in this attribute and have to be floating point values"
        },
        "scale": {
          "type": "number",
          "title": "Scale",
          "description": "A scaling factor for this class of trackers. The default value is 1.0"
        }
      },
      "required": [ "name" ],
      "description": "Defines a group of tracking devices or sensors as they are advertised by the VRPN standard. See its documentation for more information on this"
    }
  },


  "type": "object",
  "properties": {
    "masteraddress": {
      "$ref": "#/$defs/masteraddress"
    },
    "nodes": {
      "type": "array",
      "items": { "$ref": "#/$defs/node" },
      "title": "Nodes"
    },
    "capture": {
      "$ref": "#/$defs/capture",
      "title": "Capture"
    },
    "debuglog": {
      "type": "boolean",
      "title": "Debug Log",
      "description": "Determines whether the logging should include Debug level log messages. The default value is false, such that only Info level log messages or above are added to the console, the file, or the registered callback. Log messages that are not logged are discarded"
    },
    "externalcontrolport": {
      "type": "integer",
      "minimum": 0,
      "title": "External Control Port",
      "description": "If this value is set, a socket will be opened at the provided port. Messages being sent to that port will trigger a call to the callback function externalDecode. If such a callback does not exist, the incoming messages are ignored. The default behavior is that no such external port is opened. Please note that operating systems have restricted behavior when trying to open ports lower than a fixed limt. For example, Unix does not allow non-elevated users to open ports < 1024"
    },
    "firmsync": {
      "type": "boolean",
      "title": "Firm Sync",
      "description": "Determines whether the server should frame lock and wait for all client nodes or not. The default for this is false. Additionally, it is possible (and more advised) to set the frame locking on an individual node bases for the cases where not all nodes are part of a swap group or the same swap group"
    },
    "scene": {
      "$ref": "#/$defs/scene",
      "title": "Scene"
    },
    "settings": {
      "$ref": "#/$defs/settings",
      "title": "Settings"
    },
    "threadaffinity": {
      "type": "integer",
      "minimum": 0,
      "title": "Thread Affinity",
      "description": "Forces the thread affinity for the main thread of the application. On Windows, this is achieved using the SetThreadAffinityMask, but it might not be implemented on other operating systems. The default value is that no thread affinity is set for the application"
    },
    "trackers": {
      "type": "array",
      "items": {"$ref": "#/$defs/tracker" },
      "title": "Trackers"
    },
    "version": {
      "type": "integer"
    },
    "users": {
      "type": "array",
      "items": { "$ref": "#/$defs/user" },
      "title": "Users"
    }
  },
  "required": [
    "version", "masteraddress"
  ]
})|";
