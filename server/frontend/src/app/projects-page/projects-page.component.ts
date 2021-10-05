import { Component, OnInit } from '@angular/core';
import * as data from '../projects.json'
import {writeJsonFile} from 'write-json-file';
import { MatDialog, MatDialogConfig } from '@angular/material/dialog';
import { ProjModalComponent } from '../proj-modal/proj-modal.component';
import { AlertsService } from 'angular-alert-module';
import { ProjectDescModalComponent } from '../project-desc-modal/project-desc-modal.component';

@Component({
  selector: 'app-projects-page',
  templateUrl: './projects-page.component.html',
  styleUrls: ['./projects-page.component.css']
})
export class ProjectsPageComponent implements OnInit {

  
  public projects:{id:any, name:any, location:any, sensors:any}[] = (data as any).default;
  

  constructor(public matDialog: MatDialog, private alerts: AlertsService) { }

  ngOnInit(): void {
  	console.log(data)
  	this.projects.push({"id":99,"name":"CM","location":"WL","sensors":23 });
  	localStorage.setItem('projects',JSON.stringify(this.projects))

  }

  openModal() {
    const dialogConfig = new MatDialogConfig();
    // The user can't close the dialog by clicking outside its body
    dialogConfig.disableClose = true;
    dialogConfig.id = "app-proj-modal";
    dialogConfig.height = "400px";
    dialogConfig.width = "500px";
    // https://material.angular.io/components/dialog/overview
    const modalDialog = this.matDialog.open(ProjModalComponent, dialogConfig);
  }

  openProjModal(id, name, location,sensors) {
    const dialogConfig = new MatDialogConfig();

    // The user can't close the dialog by clicking outside its body
    dialogConfig.disableClose = true;
    dialogConfig.id = "app-project-desc-modal";
    dialogConfig.height = "400px";
    dialogConfig.width = "500px";
    dialogConfig.data = {
    	id: id,
    	name: name,
    	location: location,
    	sensors:sensors
    }
    // https://material.angular.io/components/dialog/overview
    const modalDialog = this.matDialog.open(ProjectDescModalComponent, dialogConfig);
  }
}
