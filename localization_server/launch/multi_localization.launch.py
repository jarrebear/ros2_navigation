import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    map_file = os.path.join(get_package_share_directory('map_server'), 'config', 'turtlebot_area_two_robots_clean.yaml')
    amcl_0_yaml = os.path.join(get_package_share_directory('localization_server'), 'config', 'tb3_0_amcl_config.yaml')
    amcl_1_yaml = os.path.join(get_package_share_directory('localization_server'), 'config', 'tb3_1_amcl_config.yaml')

    map_node = Node(package='nav2_map_server',
            executable='map_server',
            name='map_server',
            output='screen',
            parameters=[{'use_sim_time': True}, 
                        {'topic_name':"map"},
                        {'frame_id':"map"},
                        {'yaml_filename':map_file} 
                       ])

    amcl_0_node = Node(namespace='tb3_0',
            package='nav2_amcl',
            executable='amcl',
            name='amcl',
            output='screen',
            parameters=[amcl_0_yaml]
)

    amcl_1_node = Node(namespace='tb3_1',
            package='nav2_amcl',
            executable='amcl',
            name='amcl',
            output='screen',
            parameters=[amcl_1_yaml]
)

    lifecycle_manager_node = Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_localization',
            output='screen',
            parameters=[{'use_sim_time': True},
                        {'autostart': True},
                        {'bond_timeout':0.0},
                        {'node_names': ['map_server', 'tb3_0/amcl',  'tb3_1/amcl']}])

    # create and return launch description object
    return LaunchDescription(
        [
            map_node,
            amcl_0_node,
            amcl_1_node,
            lifecycle_manager_node,
        ]
    )