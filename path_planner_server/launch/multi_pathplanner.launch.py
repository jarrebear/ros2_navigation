import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    nav2_0_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'planner_server_tb3_0.yaml')
    controller_0_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'controller_tb3_0.yaml')
    bt_navigator_0_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'bt_navigator_tb3_0.yaml')
    recovery_0_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'recovery_tb3_0.yaml')
    nav2_1_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'planner_server_tb3_1.yaml')
    controller_1_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'controller_tb3_1.yaml')
    bt_navigator_1_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'bt_navigator_tb3_1.yaml')
    recovery_1_yaml = os.path.join(get_package_share_directory('path_planner_server'), 'config', 'recovery_tb3_1.yaml')

    planner_server_tb3_0 = Node(namespace='tb3_0',
            package='nav2_planner',
            executable='planner_server',
            name='planner_server',
            output='screen',
            parameters=[nav2_0_yaml])

    control_server_tb3_0 = Node(namespace='tb3_0',
            name='controller_server',
            package='nav2_controller',
            executable='controller_server',
            output='screen',
            parameters=[controller_0_yaml])

    bt_navigator_tb3_0 = Node(namespace='tb3_0',
            package='nav2_bt_navigator',
            executable='bt_navigator',
            name='bt_navigator',
            output='screen',
            parameters=[bt_navigator_0_yaml])
            
    behavior_server_tb3_0 = Node(namespace='tb3_0',
            package='nav2_behaviors',
            executable='behavior_server',
            name='recoveries_server',
            parameters=[recovery_0_yaml],
            output='screen')

    planner_server_tb3_1 = Node(namespace='tb3_1',
            package='nav2_planner',
            executable='planner_server',
            name='planner_server',
            output='screen',
            parameters=[nav2_1_yaml])

    control_server_tb3_1 = Node(namespace='tb3_1',
            name='controller_server',
            package='nav2_controller',
            executable='controller_server',
            output='screen',
            parameters=[controller_1_yaml])

    bt_navigator_tb3_1 = Node(namespace='tb3_1',
            package='nav2_bt_navigator',
            executable='bt_navigator',
            name='bt_navigator',
            output='screen',
            parameters=[bt_navigator_1_yaml])
            
    behavior_server_tb3_1 = Node(namespace='tb3_1',
            package='nav2_behaviors',
            executable='behavior_server',
            name='recoveries_server',
            parameters=[recovery_1_yaml],
            output='screen')

    lifecycle_manager_node = Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_localization',
            output='screen',
            parameters=[{'use_sim_time': True},
                        {'autostart': True},
                        {'bond_timeout': 0.0},
                        {'node_names': ['tb3_0/planner_server',
                          'tb3_0/controller_server', 'tb3_0/bt_navigator',
                          'tb3_0/recoveries_server', 'tb3_1/planner_server',
                          'tb3_1/controller_server', 'tb3_1/bt_navigator',
                          'tb3_1/recoveries_server']}])

    # create and return launch description object
    return LaunchDescription(
        [
            planner_server_tb3_0,
            control_server_tb3_0,
            bt_navigator_tb3_0,
            behavior_server_tb3_0,
            planner_server_tb3_1,
            control_server_tb3_1,
            bt_navigator_tb3_1,
            behavior_server_tb3_1,
            lifecycle_manager_node,
        ]
    )